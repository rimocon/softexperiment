/*****************各種読み込み*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../constants.h"

/****************変数宣言＆キャスト**************/
static CLIENT clients[MAX_NUM_CLIENTS];
static int num_clients;
static int num_socks;
static fd_set mask;
static CONTAINER data;

void setup_server(int, u_short);
int control_requests();
void terminate_server();

static void send_data(int, void *, int);
static int receive_data(int, void *, int);
static void handle_error(char *);

/****************セットアップ関数**************/
void setup_server(int num_cl, u_short port) { 
	int rsock, sock = 0; //ソケットと初期化
	struct sockaddr_in sv_addr, cl_addr; //socketaddr_in型構造体を利用

	fprintf(stderr, "Server setup is started.\n");

	num_clients = num_cl; //クライアントの数はnum_cl(サーバー起動時の第一引数)

	/*********************ソケット生成*********************************/	
	rsock = socket(AF_INET, SOCK_STREAM, 0); //引数はそれぞれインターネット,ストリームソケット,TCPの意

	//エラー処理
	if (rsock < 0) {
		handle_error("socket()");
	}
	//成功時のメッセージ
	fprintf(stderr, "sock() for request socket is done successfully.\n");

	sv_addr.sin_family = AF_INET; //アドレスの種類=インターネット
	sv_addr.sin_port = htons(port); //ポート番号="port"
	sv_addr.sin_addr.s_addr = INADDR_ANY; //任意のアドレスから受付可の意

	int opt = 1;
	//オプション操作??????????????????/
	setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	/******************ソケットに設定を結びつける*******************/
	if (bind(rsock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) {
		handle_error("bind()");
	}
	//成功時はfprintf
	fprintf(stderr, "bind() is done successfully.\n");

	/**********************接続ソケットの準備***********************/
	if (listen(rsock, num_clients) != 0) { //第一引数=接続受付用ソケット,第二引数=接続要求受付最大数
		handle_error("listen()");
	}
	//成功時はfprintf
	fprintf(stderr, "listen() is started.\n");

	/*****************************接続ソケットの生成と接続の確立***************************/
	int i, max_sock = 0;
	socklen_t len;
	char src[MAX_LEN_ADDR];
	for (i = 0; i < num_clients; i++) {
		len = sizeof(cl_addr);
		//accept,第一引数=接続要求受付用ソケット,第二,三引数=接続先アドレス情報とそのサイズ(NULL=指定なし)
		sock = accept(rsock, (struct sockaddr *)&cl_addr, &len);
		//例外処理
		if (sock < 0) {
			handle_error("accept()");
		}
		if (max_sock < sock) {
			max_sock = sock;
		}
		/*******************コネクションからバッファへのデータの読み込み*******************/
		if (read(sock, clients[i].name, MAX_LEN_NAME) == -1) {//第一引数=接続ソケット,第二引数=読み込み先のバッファ,第三引数=読み込みバイト数
			handle_error("read()");
		}
		clients[i].cid = i;
		clients[i].sock = sock;
		clients[i].addr = cl_addr;
		memset(src, 0, sizeof(src));
		//バイナリ形式からテキスト形式に変換
		inet_ntop(AF_INET, (struct sockaddr *)&cl_addr.sin_addr, src, sizeof(src));
		fprintf(stderr, "Client %d is accepted (name=%s, address=%s, port=%d).\n", i, clients[i].name, src, ntohs(cl_addr.sin_port));
	}

	close(rsock);

	int j;
	for (i = 0; i < num_clients; i++) {
		send_data(i, &num_clients, sizeof(int));
		send_data(i, &i, sizeof(int));
		for (j = 0; j < num_clients; j++) {
			send_data(i, &clients[j], sizeof(CLIENT));
		}
	}

	num_socks = max_sock + 1;
	FD_ZERO(&mask);
	FD_SET(0, &mask);

	for (i = 0; i < num_clients; i++) {
		FD_SET(clients[i].sock, &mask);
	}
	fprintf(stderr, "Server setup is done.\n");
}

int control_requests() {
	fd_set read_flag = mask;
	memset(&data, 0, sizeof(CONTAINER));
	/***********************ソケット通信の多重化(TCP)****************************/
	fprintf(stderr, "select() is started.\n");
	if (select(num_socks, (fd_set *)&read_flag, NULL, NULL, NULL) == -1) {

		handle_error("select()");
	}

	int i, result = 1;
	for (i = 0; i < num_clients; i++) {
		if (FD_ISSET(clients[i].sock, &read_flag)) { //ファイルディスクリプタがあるかどうか
			receive_data(i, &data, sizeof(data)); //データを受け取る
			switch (data.command) {
				case MESSAGE_COMMAND: //メッセージを出力
					fprintf(stderr, "client[%d] %s: message = %s\n", clients[i].cid, clients[i].name, data.message);
					send_data(BROADCAST, &data, sizeof(data));
					result = 1; //1ならメッセージ出力
					break;
				case QUIT_COMMAND:
					fprintf(stderr, "client[%d] %s: quit\n", clients[i].cid, clients[i].name);
					send_data(BROADCAST, &data, sizeof(data)); //ブロードキャストで送信
					result = 0; //0なら終了
					break;
				default:
					fprintf(stderr, "control_requests(): %c is not a valid command.\n", data.command);
					exit(1);
			}
		}
	}

	return result;
}

static void send_data(int cid, void *data, int size) { //メッセージ出力関数
	if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) { //ブロードキャストでない場合(個別)の時でナンバーが間違っている場合
		fprintf(stderr, "send_data(): client id is illeagal.\n");
		exit(1);
	}
	if ((data == NULL) || (size <= 0)) { //データがない場合
		fprintf(stderr, "send_data(): data is illeagal.\n");
		exit(1);
	}

	if (cid == BROADCAST) { //ブロードキャストで送信する場合
		int i;
		for (i = 0; i < num_clients; i++) { //順にクライアントに送信
			if (write(clients[i].sock, data, size) < 0) {
				handle_error("write()");
			}
		}
	} else {
		if (write(clients[cid].sock, data, size) < 0) { //ブロードキャストでない場合
			handle_error("write()");
		}
	}
}

static int receive_data(int cid, void *data, int size) { //メッセージ受信関数
	if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) { //
		fprintf(stderr, "receive_data(): client id is illeagal.\n");
		exit(1);
	}
	if ((data == NULL) || (size <= 0)) {
		fprintf(stderr, "receive_data(): data is illeagal.\n");
		exit(1);
	}

	return read(clients[cid].sock, data, size);
}

static void handle_error(char *message) {
	perror(message);
	fprintf(stderr, "%d\n", errno);
	exit(1);
}

void terminate_server(void) {
	int i;
	for (i = 0; i < num_clients; i++) {
		close(clients[i].sock);
	}
	fprintf(stderr, "All connections are closed.\n");
	exit(0);
}
