#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_NUM_STOCK 50
#define MAX_NUM_DAY 10
#define MAX_NUM_MONEY 200000
#define MAX_PRICE_STOCK 1000

#define MIN_NUM_STOCK 1
#define MIN_NUM_DAY 2
#define MIN_NUM_MONEY 1
#define MIN_PRICE_STOCK 1

int main(int argc, char* argv[]) {
	if(argc == 2) {
		char* var = argv[1];
		
		if(var[0] == '-') {
			size_t len = strlen(argv[1]);
			if(len != 2) {
				exit(1);
			}
			if(var[len - 1] == 'h') {
				printf("*******************************\n");
				printf("*********  help page  *********\n");
				printf("*******************************\n");
				printf("*** arguments are not mandatory.\n");
				printf("*** if you just run the program without arguments,\n");
				printf("*** it will run with random values.\n\n");
				printf("*** the random max values are \n");
				printf("*** MAX_NUM_STOCK: %d, ",MAX_NUM_STOCK);
				printf("MAX_NUM_DAY: %d\n",MAX_NUM_DAY);
				printf("*** MAX_NUM_MONEY: %d, ",MAX_PRICE_STOCK);
				printf(" MAX_PRICE_STOCK: %d\n\n",MAX_PRICE_STOCK);
				printf("*** the random min values are \n");
				printf("*** MIN_NUM_STOCK: %d, ",MIN_NUM_STOCK);
				printf(" MIN_NUM_DAY: %d\n",MIN_NUM_DAY);
				printf("*** MIN_NUM_MONEY: %d, ",MIN_NUM_MONEY);
				printf(" MIN_PRICE_STOCK: %d\n\n",MIN_PRICE_STOCK);
				printf("*** IF you want to customize the values,\n");
				printf("*** the order is the following\n");
				printf("*** argument 1 : num_stock\n");
				printf("*** argument 2 : num_day\n");
				printf("*** argument 3 : num_money\n");
				printf("*** argument 4 : min_price\n");
				printf("*** argument 5 : max_price\n");
				printf("*** example: to set the numbers\n");
				printf("*** 4 stocks, 3days, 100 money, 10 min stock price, 50 max stock price\n");
				printf("*** main.exe 4 3 100 10 50\n");
				return 0;
			} else {
				exit(1);
			}
		}
	}
	
	srand(time(NULL));
	long num_stock,num_day,num_money,min_price,max_price;
	num_stock = (rand() % (MAX_NUM_STOCK + 1)) + MIN_NUM_STOCK;
	num_day = (rand() % (MAX_NUM_DAY + 1)) + MIN_NUM_DAY;
	num_money = (rand() % (MAX_NUM_MONEY + 1)) + MIN_NUM_MONEY;	
	min_price = MIN_PRICE_STOCK;
	max_price = MAX_PRICE_STOCK;
	if(argc > 1) {
		num_stock = strtol(argv[1],NULL,10);
	}
	if(argc > 2) {
		num_day = strtol(argv[2],NULL,10);
	}
	if(argc > 3) {
		num_money = strtol(argv[3],NULL,10);
	}
	if(argc > 4) {
		min_price = strtol(argv[4],NULL,10);
	}
	if(argc > 5) {
		max_price = strtol(argv[5],NULL,10);
	}

	printf("num stock: %d\n",num_stock);
	printf("num day: %d\n",num_day);
	printf("num money: %d\n",num_money);
	printf("\n");
	FILE *file;
	file = fopen("case.txt","w");
	if(file == NULL) {
		printf("file cannnot be opened");
		exit(1);
	}
	fprintf(file,"%d ", num_stock);
	fprintf(file,"%d ", num_day);
	fprintf(file,"%d", num_money);
	fprintf(file,"\n");
	for(int i = 0; i < num_stock; ++i) {
		for(int j = 0; j < num_day; ++j) {
			int p = (rand() % (max_price + 1)) + min_price;
			fprintf(file,"%d ", p);
		}
		fprintf(file,"\n");
	}
	
	fclose(file);
	return 0;
}
