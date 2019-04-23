extern int count;

void display(void)
{
  int i;
  for ( i = 0; i< count; i++){
    puts(":::::::::");
  }
  count+=5;
}
