#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int curr_row;
	int curr_col;
	char curr_dir;
}Farmer;

void print_land(Farmer cse_farmer, char planted[8][8], char watered[8][8]);
int seed_exist(char seed_name[5], char seed, int seed_num);
void watering(Farmer cse_farmer, char watered[8][8]);
void planting(int row, int col, Farmer cse_farmer, char planted[8][8], int seed_amount[5], char seed, char seed_name[5], int seed_num);
int getSeedIndex(char seed_name[5], char name, int seed_num);
int seedStatus(char seed_name[5], char name, int seed_num);
void advancing_to_next_day(char planted[8][8], char watered[8][8]);
int isGrown(char name);
void harvest(Farmer cse_farmer, char planted[8][8], int seed_amount[5], char seed_name[5], int seed_num);
int trading(char src_name, int src_amt, char dst_name, char seed_name[5], int seed_amount[5], int seed_num);
void droughts(int min_num_plants_to_die, char planted[8][8], int seeds_to_be_die[8][8]);
void windStorm(int min_num_plants_to_survive, char planted[8][8], int seeds_to_be_die[8][8]);
int surroundedNum(int row, int col, char planted[8][8]);

int main(){
	Farmer cse_farmer = { .curr_row = 0, .curr_col = 0, .curr_dir = '>' };
	int seed_num, seed_amount[5] = {}, day = 1, seeds_to_be_die[8][8] = {};
	char seed_name[5], planted[8][8]={}, watered[8][8]={}, command;
        printf("Welcome to CSE Valley, farmer!\n");
        printf("Congratulations, you have received 60 seeds.\n");
        printf("How many different seeds do you wish to have? ");
	
  //Enter the number of seeds until the input value is an integer between 1 and 5.
  for(;;){
		scanf( "%d", &seed_num );
		if(seed_num < 1 || seed_num > 5) {
			printf("Please enter the number between 1 and 5: ");	
		} else {
			break;
		}
	}
  //Calculate the amount of all seeds.
	for(int i = 0; i < seed_num; i ++) {
		seed_amount[i] = 60 / seed_num;
	}
	printf("Enter the names of the seeds to be given: \n");
	for( int i = 0; i < seed_num ; i ++) {
		int same_name_flag = 0;
		scanf("			%c", &seed_name[i]);
    //If the seed name is not lowercase, re-enter the seed name.
		if((int)seed_name[i] < 97 || (int)seed_name[i] > 122) {
			printf("  Seed name has to be a lowercase letter\n");
			i --;
			continue;
		}
    //Check if the input seed already exists.
		for( int j = 0; j < i; j ++){
			if(seed_name[i] == seed_name[j]) {
				same_name_flag = 1;
				break;	
			}
		}
    //If the seed name is exist, re-enter the seed name.
		if(same_name_flag == 1){
			printf("  You can't input the same seed name\n");
			i --;
			continue;
		}
	}
	
	printf("Game Started!\nEnter command: ");
	while(scanf("%c",  &command) != EOF){
    //If the command is 'a', output the amount of all seeds.
		if(command == 'a') {
			printf("  Seeds at your disposal:\n");
			for(int i = 0;i < seed_num;i ++){      
				printf("  - %d seed(s) with the name '%c'\n", seed_amount[i], seed_name[i]);
			}
		}
    //If the command is 's', output the amount of special seed.
		else if(command == 's') {
			char special_seed_name[10];
      //Input the name of special seed.
			scanf("%s", special_seed_name);
			if(seedStatus(seed_name, special_seed_name[0], seed_num) == 0) { //If the seed name is lowercase letter and seed is exist...
				printf("  There are %d seeds with the name '%c'\n", seed_amount[getSeedIndex(seed_name, special_seed_name[0], seed_num)], special_seed_name[0]);	
			} else if(seedStatus(seed_name, special_seed_name[0], seed_num) == 1){ //If the seed name is not lowercase...
				printf("  Seed name has to be a lowercase letter\n");
			} else if(seedStatus(seed_name, special_seed_name[0], seed_num) == 2){ //If the seed is not exist...
				printf("  There is no seed with the name '%c'\n", special_seed_name[0]);
			}
		}
    //If the command is 'l', the function to draw the ground is called.
		else if(command == 'l') {
			print_land(cse_farmer, planted, watered);
		}
    //If the command is '^', move upwards. 
		else if(command == '^') {
      //If the current row of farmer is 0, do nothing. 
			if(cse_farmer.curr_row == 0){
				printf("Enter command: ");
				continue ;
			}
      //If the current direction of farmer is '^', move upwoards.
			if(cse_farmer.curr_dir == '^') cse_farmer.curr_row -= 1;
			cse_farmer.curr_dir = '^';
		}
		else if(command == 'v') {
      //If the current row of farmer is 7, do nothing.
			if(cse_farmer.curr_row == 7){
				printf("Enter command: ");
				continue ;
			}
      //If the current direction of farmer is 'v', move downwoards.
			if(cse_farmer.curr_dir == 'v') cse_farmer.curr_row += 1;
			cse_farmer.curr_dir = 'v';
		}
		else if(command == '<') {
      //If the current col of farmer is 0, do nothing.
			if(cse_farmer.curr_col == 0){
				printf("Enter command: ");
				continue ;
			}
      //If the current direction of farmer is '<', move left.
			if(cse_farmer.curr_dir == '<') cse_farmer.curr_col -= 1;
			cse_farmer.curr_dir = '<';
		}
		else if(command == '>') {
      //If the current col of farmer is 0, do nothing.
			if(cse_farmer.curr_col == 7){
				printf("Enter command: ");
				continue ;
			}
      //If the current direction of farmer is '>', move left.
			if(cse_farmer.curr_dir == '>') cse_farmer.curr_col += 1;
			cse_farmer.curr_dir = '>';
		}
		else if(command == 'o'){
			char sub_command[10];
			scanf("%s", sub_command);
      //If the command is 'o' and the subcommand is 'w', call function to do watering.
			if(sub_command[0] == 'w'){
				watering(cse_farmer, watered);
			}
      //If the command is 'o' and the subcommand is 'w', call function to do planting.
			if(sub_command[0] == 'p'){
				char name[10];
				scanf("%s", name);
				planting(cse_farmer.curr_row, cse_farmer.curr_col, cse_farmer, planted, seed_amount, name[0], seed_name, seed_num);
			}
		}
		else if(command == 'p'){
			char name[10];
			scanf("%s", name);
			if(seedStatus(seed_name, name[0], seed_num) == 1) {
				printf("  Seed name has to be a lowercase letter");
				printf("\n");
			} else if(seedStatus(seed_name, name[0], seed_num) == 2) {
				printf("  There is no seed with the name '%c'", name[0]);
				printf("\n");
			} else if(cse_farmer.curr_dir == '^' || cse_farmer.curr_dir == '<') {
				printf("  You cannot scatter seeds ^ or <");
				printf("\n");
			} else if(seedStatus(seed_name, name[0], seed_num) == 0 && seed_amount[getSeedIndex(seed_name, name[0], seed_num)] != 0) {
				if(cse_farmer.curr_dir == '>'){
					planting(cse_farmer.curr_row, cse_farmer.curr_col - 1, cse_farmer, planted, seed_amount, name[0], seed_name, seed_num);
					for(int i = cse_farmer.curr_col;i < 8; i ++){
						if(seed_amount[getSeedIndex(seed_name, name[0], seed_num)] != 0) 
							planting(cse_farmer.curr_row, i, cse_farmer, planted, seed_amount, name[0], seed_name, seed_num);
					}
				}
				if(cse_farmer.curr_dir == 'v'){
					planting(cse_farmer.curr_row - 1, cse_farmer.curr_col, cse_farmer, planted, seed_amount, name[0], seed_name, seed_num);
					for(int i = cse_farmer.curr_row;i < 8; i ++){
						if(seed_amount[getSeedIndex(seed_name, name[0], seed_num)] != 0)
							planting(i, cse_farmer.curr_col, cse_farmer, planted, seed_amount, name[0], seed_name, seed_num);
					}
				}
			}
		}
    //If the command is 'o' and the subcommand is 'w', call function to do square watering.
		else if(command == 'w')  {
			int square_size;
			scanf("%d", &square_size);
			if(square_size < 0) {
				printf("  The size argument needs to be a non-negative integer\nEnter command: ");
				continue ;
			}
			for(int i = cse_farmer.curr_row - square_size; i <= cse_farmer.curr_row + square_size; i ++)
				for(int j = cse_farmer.curr_col - square_size; j <= cse_farmer.curr_col + square_size; j ++){
					if(i < 0 || j < 0 || i > 7 || j > 7) continue;
					watered[i][j] = 'W';
				}
		}
    //If the command is 'o' and the subcommand is 'w', call function to advance to the next day.
		else if(command == 'n') {
			advancing_to_next_day(planted, watered);
      day += 1;
			printf("  Advancing to the next day... Day %d, let's go!\n", day);
			cse_farmer.curr_dir = '>';
		}
    //If the command is 'o' and the subcommand is 'w', call function to do harvesting.
		else if(command == 'h') {
			harvest(cse_farmer, planted, seed_amount, seed_name, seed_num);
		}
    //If the command is 'o' and the subcommand is 'w', call function to do trading.
		else if(command == 't') {
			char src_name[10], dst_name[10];
			int src_amt;
			scanf("%s %d %s", src_name, &src_amt, dst_name);
			if(seedStatus(seed_name, src_name[0], seed_num) == 1 || seedStatus(seed_name, dst_name[0], seed_num) == 1){
				printf("  Seed name has to be a lowercase letter");
				printf("\n");
			}
			else if(trading(src_name[0], src_amt, dst_name[0], seed_name, seed_amount, seed_num) == 1) seed_num ++;
		}
		else if(command == 'd') {
			char sub_command[10];
			scanf("%s", sub_command);
			if(sub_command[0] == 'd') {
				int min_num_plants_to_die;
				scanf("%d", &min_num_plants_to_die);
				droughts(min_num_plants_to_die, planted, seeds_to_be_die);
			}
			if(sub_command[0] == 'w') {
				int min_num_plants_to_survive;
				scanf("%d", &min_num_plants_to_survive);
				windStorm(min_num_plants_to_survive, planted, seeds_to_be_die);
			}
		} else {
			continue;
		}
		printf("Enter command: ");
	}
	return 0;
};

//Function to draw land.
void print_land(Farmer cse_farmer, char planted[8][8], char watered[8][8] ){
	printf("|---|---|---|---|---|---|---|---|\n");
	for(int i = 0; i < 8; i ++) {
		if(cse_farmer.curr_row == i){ //If the loop index is same as the current row of farmer...
			for(int j = 0; j < 3;j ++) {
				for(int k = 0; k < 8;k ++){
					printf("|");
					if(j == 0) {
						if((int)planted[i][k] != 0) printf("%c", planted[i][k]);
							else printf(" ");
						if(cse_farmer.curr_dir == '^' && cse_farmer.curr_col == k) printf("^");
							else printf(" ");
						if((int)watered[i][k] != 0) printf("%c", watered[i][k]);
							else printf(" ");
					}
					if(j == 1) {
						if(cse_farmer.curr_dir == '<' && cse_farmer.curr_col == k) printf("<");
							else printf(" ");
						if(cse_farmer.curr_col == k) printf("f");
							else printf(" ");
						if(cse_farmer.curr_dir == '>' && cse_farmer.curr_col == k) printf(">");
							else printf(" ");
					}
					if(j == 2) {
						if(cse_farmer.curr_dir == 'v' && cse_farmer.curr_col == k) printf(" v ");
							else printf("   ");
					}
				}
				printf("|\n");
			}
		} else {
			for(int j = 0; j < 8;j ++){
				if((int)planted[i][j] != 0 ) printf("|%c ", planted[i][j]);
				else printf("|  ");
				if((int)watered[i][j] != 0 ) printf("%c", watered[i][j]);
				else printf(" ");
			}	
			printf("|\n");
		}
		printf("|---|---|---|---|---|---|---|---|\n");
	}
	return;
}

//Function to check if the input seed exists.
int seed_exist(char seed_name[5], char seed, int seed_num){
	int exist = 0;
	for(int i = 0; i < seed_num; i ++){
		if(seed == seed_name[i]) {
			exist = 1;
			break;
		}
	}
	return exist;	
}

//Function to do watering.
void watering(Farmer cse_farmer, char watered[8][8]){
	int row = cse_farmer.curr_row;
	int col = cse_farmer.curr_col;
	if(cse_farmer.curr_dir == '^') {
		if(row > 0) watered[row - 1][col] = 'W';
	} else if(cse_farmer.curr_dir == 'v') {
		if(row < 7) watered[row + 1][col] = 'W';
	} else if(cse_farmer.curr_dir == '<') {
		if(col > 0) watered[row][col - 1] = 'W';
	} else if(cse_farmer.curr_dir == '>') {
		if(col < 7) watered[row][col + 1] = 'W';
	}
}

//Function to do planting.
void planting(int row, int col, Farmer cse_farmer, char planted[8][8], int seed_amount[5], char seed, char seed_name[5], int seed_num){
	if(cse_farmer.curr_dir == '^') {
		if(row > 0) {
				planted[row - 1][col] = seed;
				seed_amount[getSeedIndex(seed_name, seed, seed_num)] -= 1;
			}
	} else if(cse_farmer.curr_dir == 'v') {
		if(row < 7) {
				planted[row + 1][col] = seed;
				seed_amount[getSeedIndex(seed_name, seed, seed_num)] -= 1;
			}
	} else if(cse_farmer.curr_dir == '<') {
		if(col > 0) {
				planted[row][col - 1] = seed;
				seed_amount[getSeedIndex(seed_name, seed, seed_num)] -= 1;
			}
	} else if(cse_farmer.curr_dir == '>') {
		if(col < 7) {
				planted[row][col + 1] = seed;
				seed_amount[getSeedIndex(seed_name, seed, seed_num)] -= 1;
			}
	}
}

//Function to get the index of seed.
int getSeedIndex(char seed_name[5], char name, int seed_num) {
	for(int i = 0;i < seed_num;i ++) {
		if(seed_name[i] == name) return i;
	}
	return -1;
}

//Function to check the status of seed.
int seedStatus(char seed_name[5], char name, int seed_num) {
	if((int)name < 97 || (int)name > 122) { //If seed name is not lowercase letter...
		return 1;
	} else {
		if(seed_exist(seed_name, name, seed_num) == 1) { //If seed name is exist...
			return 0;
		} else {
			return 2;
		}
	} 
}

//Function to advance to the next day.
void advancing_to_next_day(char planted[8][8], char watered[8][8]) {
	for( int i = 0; i < 8; i ++){
		for( int j = 0; j < 8; j ++){
			if((int)planted[i][j] != 0 && watered[i][j] == 'W'){
				planted[i][j] = (char)((int)planted[i][j] - 32);
				watered[i][j] = ' ';
			} else {
				planted[i][j] = ' ';
				watered[i][j] = ' ';
			}
		}
	}
}

//Function to do harvesting.
void harvest(Farmer cse_farmer, char planted[8][8], int seed_amount[5], char seed_name[5], int seed_num) {
	int row = cse_farmer.curr_row;
	int col = cse_farmer.curr_col;
	if(cse_farmer.curr_dir == '^') {
		if(row > 0) {
				if(isGrown(planted[row - 1][col]) == 1){
					printf("  Plant '%c' was harvested, resulting in 5 '%c' seed(s)\n", planted[row - 1][col], (char)((int)planted[row - 1][col] + 32));
					seed_amount[getSeedIndex(seed_name, (char)((int)planted[row - 1][col] + 32), seed_num)] += 5;
					planted[row - 1][col] = ' ';
				}
			}
	} else if(cse_farmer.curr_dir == 'v') {
		if(row < 7) {
				if(isGrown(planted[row + 1][col]) == 1){
					printf("  Plant '%c' was harvested, resulting in 5 '%c' seed(s)\n", planted[row + 1][col], (char)((int)planted[row + 1][col] + 32));
					seed_amount[getSeedIndex(seed_name, (char)((int)planted[row + 1][col] + 32), seed_num)] += 5;
					planted[row + 1][col] = ' ';
				}
			}
	} else if(cse_farmer.curr_dir == '<') {
		if(col > 0) {
				if(isGrown(planted[row][col - 1]) == 1){
					printf("  Plant '%c' was harvested, resulting in 5 '%c' seed(s)\n", planted[row - 1][col], (char)((int)planted[row][col - 1] + 32));
					seed_amount[getSeedIndex(seed_name, (char)((int)planted[row][col - 1] + 32), seed_num)] += 5;
					planted[row - 1][col] = ' ';
				}
			}
	} else if(cse_farmer.curr_dir == '>') {
		if(col < 7) {
				if(isGrown(planted[row][col + 1]) == 1){
					printf("  Plant '%c' was harvested, resulting in 5 '%c' seed(s)\n", planted[row][col + 1], (char)((int)planted[row][col + 1] + 32));
					seed_amount[getSeedIndex(seed_name, (char)((int)planted[row][col + 1] + 32), seed_num)] += 5;
					planted[row][col + 1] = ' ';
				}
			}
	}
}

//Function to check if the seed has grown.
int isGrown(char name){
	if((int)name < 65 || (int)name > 90) return 0;
	else return 1;
}

//Function to do trading.
int trading(char src_name, int src_amt, char dst_name, char seed_name[5], int seed_amount[5], int seed_num) {
	if(src_amt < 0){
		printf("  You can't trade negative seeds");
		printf("\n");
		return 0;
	} else if(seed_exist(seed_name, src_name, seed_num) == 0) {
		printf("  You don't have the seeds to be traded");
		printf("\n");
		return 0;
	} else if(seed_amount[getSeedIndex(seed_name, src_name, seed_num)] < src_amt){
		printf("  You don't have enough seeds to be traded");
		printf("\n");
		return 0;
	} else if(seed_exist(seed_name, dst_name, seed_num) == 0) {
		seed_name[seed_num] = dst_name;
		seed_amount[seed_num] = src_amt;
		seed_amount[getSeedIndex(seed_name, src_name, seed_num)] -= src_amt;
		return 1;
	} else{
		seed_amount[getSeedIndex(seed_name, src_name, seed_num)] -= src_amt;
		seed_amount[getSeedIndex(seed_name, dst_name, seed_num)] += src_amt;
		return 0;
	}
}

void droughts(int min_num_plants_to_die, char planted[8][8], int seeds_to_be_die[8][8]) {
	for(int i = 0; i < 8; i ++)
		for(int j = 0; j < 8; j ++)
			if(surroundedNum(i, j, planted) >= min_num_plants_to_die ) seeds_to_be_die[i][j] = 1;
	for(int i = 0; i < 8; i ++)
		for(int j = 0; j < 8; j ++)
			if(seeds_to_be_die[i][j] == 1 ) planted[i][j] = ' ';	
}

void windStorm(int min_num_plants_to_survive, char planted[8][8], int seeds_to_be_die[8][8]) {
	for(int i = 0; i < 8; i ++)
		for(int j = 0; j < 8; j ++)
			if(surroundedNum(i, j, planted) < min_num_plants_to_survive ) seeds_to_be_die[i][j] = 1;
	for(int i = 0; i < 8; i ++)
		for(int j = 0; j < 8; j ++)
			if(seeds_to_be_die[i][j] == 1 ) planted[i][j] = ' ';
}

//Function to count the number of enclosed seeds.
int surroundedNum(int row, int col, char planted[8][8]){
	int num = 0;
	for(int i = row - 1;i <= row + 1; i ++)
		for(int j = col - 1; j <= col + 1; j ++){
			if((i == row && j == col) || i < 0 || j < 0 || i > 7 || j > 7) continue;
			if((int)planted[i][j] >=97 && (int)planted[i][j] <= 122) {
				num ++;
			}
		}
	return num;
}