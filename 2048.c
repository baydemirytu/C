#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

int score;

void random_assign(int[4][4]);
void print_game(int[4][4]);
void play_game(int[4][4], int);
void shift_up(int[4][4]);
void shift_left(int[4][4]);
void shift_down(int[4][4]);
void shift_right(int[4][4]);
void combine_up(int[4][4]);
void combine_left(int[4][4]);
void combine_down(int[4][4]);
void combine_right(int[4][4]);
void restart(int[4][4]);
void copy(int[4][4], int[4][4]);
void wait(char[30]);
int compare(int[4][4], int[4][4]);
int game_over(int[4][4]);


int main() {
	system("title 2048");
    int game_area[4][4] = {};
    int random_row, random_column;
    int gameover = 0;
	
	printf("\n\n\n\t\t\t");
	char s1[30] = "WELCOME TO 2048 GAME";
	wait(s1);
	
	printf("\n\n\n\t\t\t");
	char s2[40] = "Press Any Key To Continue...";
	wait(s2);
	char key;
	key = getch();
	system("cls");
		
    srand(time(NULL));
    random_row = rand() % 4;
    random_column = rand() % 4;
    game_area[random_row][random_column] = (rand() % 2 + 1) * 2;

    random_assign(game_area);
    print_game(game_area);

    gameover = game_over(game_area);
	
	char key2;
	char s4[50] = "Game is over... ^-^\n		  ";
	char s5[50] = "Restarting...";
    while (1) {

        play_game(game_area, gameover);
		
        printf("\n\n\n		  ");		  
		wait(s4);		  

        wait(s2);
        key2 = getch();
        
		printf("\n  		  ");
        wait(s5);
        
        restart(game_area);
        system("cls");
        print_game(game_area);
    }

    return 0;
}

void wait(char s1[30]){
	
	int i = 0;
	int j;
	while(s1[i] != '\0'){
		
		printf("%c", s1[i]);
		i++;
		for(j = 0; j<22000000; j++){
		}
	}
}

void random_assign(int game_area[4][4]) {

    int random_row = rand() % 4;
    int random_column = rand() % 4;
    int i, j, empty_cell = 16;

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            if (game_area[i][j] != 0) {

                empty_cell--;

            }
        }
    }
    if (empty_cell != 0) {

        while (game_area[random_row][random_column] != 0) {

            random_row = rand() % 4;
            random_column = rand() % 4;

        }
        game_area[random_row][random_column] = (rand() % 2 + 1) * 2;
    }

}

void print_game(int game_area[4][4]) {
    int i, j;
    printf("\n\n\n\n\n\n\t\t");
    printf(" ____ ____ ____ ____ \n\t\t");

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            if (game_area[i][j] != 0) {

                printf("|%4d", game_area[i][j]);

            } else if (game_area[i][j] == 0) {

                printf("|    ");

            }
        }
        printf("|\n");
        printf("\t\t ____ ____ ____ ____ ");
        printf("\n\t\t");
    }
    printf("\n\n\t\tScore: %d", score);
    printf("\n\n\t\tPress r to restart");
    printf("\n");
}

int game_over(int game_area[4][4]) {
    int i = 0, j = 0, empty_cell = 16, gameover = 0;

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            if (game_area[i][j] != 0) {

                empty_cell--;

            }
        }
    }

    if (empty_cell != 0) {
        return gameover;
    } else {
        int test[4][4] = {};
        int gameover_array[4];
        copy(game_area, test);
        shift_up(test);
        combine_up(test);
        gameover = compare(game_area, test);
        gameover_array[0] = gameover;

        copy(game_area, test);
        shift_left(test);
        combine_left(test);
        gameover = compare(game_area, test);
        gameover_array[1] = gameover;

        copy(game_area, test);
        shift_down(test);
        combine_down(test);
        gameover = compare(game_area, test);
        gameover_array[2] = gameover;

        copy(game_area, test);
        shift_right(test);
        combine_right(test);
        gameover = compare(game_area, test);
        gameover_array[3] = gameover;

        if ((gameover_array[0] == 1) && (gameover_array[1] == 1) && (gameover_array[2] == 1) && (gameover_array[3] == 1)) {
            return 1;
        } else {
            return 0;
        }
    }
}

void play_game(int game_area[4][4], int gameover) {
    char pressed_key[1];
    int test[4][4];
    int test_var;
    char arrow;
    
    while (gameover == 0) {
		
		arrow = getch();
        if (arrow == 'r'){
        	restart(game_area);
		}
		switch(arrow){
     
			case UP:
		  	{
			  
			  	copy(game_area, test);
            	shift_up(game_area);
            	combine_up(game_area);
            	test_var = compare(game_area, test);
		  	
				if (test_var == 0){
               		random_assign(game_area);
            	}
		  	
				break;
		  	}
	   
	        case LEFT:
	        {
					
	        	copy(game_area, test);
	            shift_left(game_area);
	            combine_left(game_area);
	            test_var = compare(game_area, test);
	            if (test_var == 0) {
	                random_assign(game_area);
	            }
	        	
				break;
			}
 
       		case DOWN:
       		{
				   	
				copy(game_area, test);
            	shift_down(game_area);
            	combine_down(game_area);
            	test_var = compare(game_area, test);
            	if (test_var == 0) {
                	random_assign(game_area);
            	}	
    
				break;
			}

        	case RIGHT:
        	{
					
        		copy(game_area, test);
            	shift_right(game_area);
            	combine_right(game_area);
            	test_var = compare(game_area, test);
            	if (test_var == 0) {
               	 random_assign(game_area);
            	}

				break;
			}
        }

        system("cls");
        print_game(game_area);
        gameover = game_over(game_area);
    }
}

void shift_up(int game_area[4][4]) {
    int i, j, tmp_raw;

    for (i = 1; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            tmp_raw = i;

            while ((game_area[i][j] != 0) && (game_area[tmp_raw - 1][j] == 0) && (tmp_raw > 0)) {
                tmp_raw--;
            }

            if (tmp_raw != i) {
                game_area[tmp_raw][j] = game_area[i][j];
                game_area[i][j] = 0;
            }

        }
    }
}

void shift_left(int game_area[4][4]) {
    int i, j, tmp_column;

    for (i = 0; i < 4; i++) {

        for (j = 1; j < 4; j++) {

            tmp_column = j;

            while ((game_area[i][j] != 0) && (game_area[i][tmp_column - 1] == 0) && (tmp_column > 0)) {
                tmp_column--;
            }

            if (tmp_column != j) {
                game_area[i][tmp_column] = game_area[i][j];
                game_area[i][j] = 0;
            }

        }
    }
}

void shift_down(int game_area[4][4]) {
    int i, j, tmp_raw;

    for (i = 2; i >= 0; i--) {

        for (j = 0; j < 4; j++) {

            tmp_raw = i;

            while ((game_area[i][j] != 0) && (game_area[tmp_raw + 1][j] == 0) && (tmp_raw < 3)) {
                tmp_raw++;
            }

            if (tmp_raw != i) {
                game_area[tmp_raw][j] = game_area[i][j];
                game_area[i][j] = 0;
            }

        }
    }
}

void shift_right(int game_area[4][4]) {
    int i, j, tmp_column;

    for (i = 0; i < 4; i++) {

        for (j = 2; j >= 0; j--) {

            tmp_column = j;

            while ((game_area[i][j] != 0) && (game_area[i][tmp_column + 1] == 0) && (tmp_column < 3)) {
                tmp_column++;
            }

            if (tmp_column != j) {
                game_area[i][tmp_column] = game_area[i][j];
                game_area[i][j] = 0;
            }

        }
    }
}

void combine_up(int game_area[4][4]) {
    int i, j;

    for (i = 0; i < 3; i++) {

        for (j = 0; j < 4; j++) {

            if (game_area[i][j] == game_area[i + 1][j]) {
                score += game_area[i][j];
				game_area[i][j] *= 2;
                game_area[i + 1][j] = 0;
            }
        }
    }
    shift_up(game_area);
}

void combine_left(int game_area[4][4]) {
    int i, j;

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 3; j++) {

            if (game_area[i][j] == game_area[i][j + 1]) {
                score += game_area[i][j];
				game_area[i][j] *= 2;
                game_area[i][j + 1] = 0;
            }
        }
    }
    shift_left(game_area);
}

void combine_down(int game_area[4][4]) {
    int i, j;

    for (i = 3; i > 0; i--) {

        for (j = 0; j < 4; j++) {

            if (game_area[i][j] == game_area[i - 1][j]) {
                score += game_area[i][j];
				game_area[i][j] *= 2;
                game_area[i - 1][j] = 0;
            }
        }
    }
    shift_down(game_area);
}

void combine_right(int game_area[4][4]) {
    int i, j;

    for (i = 0; i < 4; i++) {

        for (j = 3; j > 0; j--) {

            if (game_area[i][j] == game_area[i][j - 1]) {
				score += game_area[i][j];
                game_area[i][j] *= 2;
                game_area[i][j - 1] = 0;

            }
        }
    }
    shift_right(game_area);
}

void restart(int game_area[4][4]) {
    int i, j;

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            game_area[i][j] = 0;

        }
    }
    random_assign(game_area);
    random_assign(game_area);
    score = 0;
}

void copy(int game_area[4][4], int test[4][4]) {
    int i, j;

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            test[i][j] = game_area[i][j];

        }
    }
}

int compare(int game_area[4][4], int test[4][4]) {
    int i = 0, j = 0, same_counter = 0;

    for (i = 0; i < 4; i++) {

        for (j = 0; j < 4; j++) {

            if (game_area[i][j] == test[i][j]) {

                same_counter++;

            }
        }
    }
    if (same_counter == 16) {
        return 1;
    } else {
        return 0;
    }

}
