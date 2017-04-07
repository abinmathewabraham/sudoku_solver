#include<stdio.h>
#include<stdlib.h>

struct backtracking_state
{
	int row;				  //row index of the element which is guessed 
	int col;				  //col index of the element which is guessed
	int *possible_values;     //array of possible values for that element when initially guessed
	int num_pos_values;       //total number of possible values for that element when initially guessed
	int curr_value_index;     //index of the possible value array whose value is considered presently
	int *modified_indices;    //array of indices modified during guessing
	int num_modified_indices; //total number of indices modified
};

int matrix[9][9];
	    
int remaining = 0;

void get_input();
void solve();
void display();
int possible_values(int *,int,int,int *);
int check_in_row_col_square(int *,int,int,int);
void push(struct backtracking_state *);
void pop();

struct backtracking_state **backtracking_stack;
int backtracking_stack_ptr;

int main()
{
	backtracking_stack = (struct backtracking_state **) malloc(sizeof(struct backtracking_state *));
	backtracking_stack_ptr = 0;
	
	get_input();
	printf("\nRemaining : %d\n",remaining);
	solve();
	display(); 	
	return 0;
}

void get_input()
{
	int i,j;
	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)
		{
			scanf("%d",&matrix[i][j]);
			remaining += (matrix[i][j] == -1 ? 1 : 0);
		}
}

void display()
{
	int i,j;
	printf("\n\n##################################################################################\n\n");
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
			printf("%d   ",matrix[i][j]);
		printf("\n");		
	}
}


void solve()
{

	int prev_remaining = 0;
	int i,j;
	int num_pos_values;
	int *value_array;
	int one_value_flag;
	int temp_count;
	struct backtracking_state *bktr_state;
	
	int count = 0;
	
	while(remaining)
	{
	
		count++;
		if(prev_remaining == remaining) //this is true when there is no change in the state of matrix; we should guess when we can't find in normal method; we do guessing inside this condition
		{
				
			one_value_flag = 0;
					
			for(i = 0; i < 9; i++)
			{
				for(j = 0; j < 9; j++)
				{
					if(matrix[i][j] == -1)
					{
						
						printf("\nguessing....\n");
						one_value_flag = 1;
						value_array = (int *) malloc(sizeof(int));
						num_pos_values = possible_values((int *)matrix,i,j,value_array);
						bktr_state = (struct backtracking_state *) malloc(sizeof(struct backtracking_state));
						bktr_state->row = i;
						bktr_state->col = j;
						bktr_state->possible_values = value_array;
						bktr_state->num_pos_values = num_pos_values;
						bktr_state->curr_value_index = 0;
						
						matrix[i][j] = *value_array;
						remaining--;
						
						bktr_state->modified_indices = (int *) malloc(sizeof(int));
						*(bktr_state->modified_indices) = i*9+j;
						bktr_state->num_modified_indices = 1;
						
						push(bktr_state);
						
						break;
					}
				}
				
				if(one_value_flag)
				{
					break;
				}
			}
		}
		
		prev_remaining = remaining;
		
		one_value_flag = 0;
		
		for(i = 0; i < 9; i++)
		{
			for(j = 0; j < 9; j++)
			{
				if(matrix[i][j] == -1)
				{
					value_array = (int *) malloc(sizeof(int));
					if((num_pos_values = possible_values((int *)matrix,i,j,value_array)) == 1)
					{
						
						matrix[i][j] = *value_array;
						remaining--;
						one_value_flag = 1;
						
						if(backtracking_stack_ptr > 0)
						{
							bktr_state = *(backtracking_stack+backtracking_stack_ptr-1);
							bktr_state->modified_indices = (int *) realloc(bktr_state->modified_indices,sizeof(int)*(bktr_state->num_modified_indices+1));
							*(bktr_state->modified_indices+bktr_state->num_modified_indices) = i*9+j;
							bktr_state->num_modified_indices++;
						}
						break;
					}
					
					if(!num_pos_values)
					{
						
						printf("\nwrong guess\n");
						bktr_state = *(backtracking_stack+backtracking_stack_ptr-1);
						while(bktr_state->curr_value_index == bktr_state->num_pos_values-1)
						{
							for(temp_count = 0; temp_count < bktr_state->num_modified_indices; temp_count++)
							{
								matrix[(*(bktr_state->modified_indices+temp_count))/9][(*(bktr_state->modified_indices+temp_count))%9] = -1;
								remaining++;
							}
							pop();
							bktr_state = *(backtracking_stack+backtracking_stack_ptr-1);
						}
						
						
						for(temp_count = 0; temp_count < bktr_state->num_modified_indices; temp_count++)
						{
							matrix[(*(bktr_state->modified_indices+temp_count))/9][(*(bktr_state->modified_indices+temp_count))%9] = -1;
							remaining++;
						}
						bktr_state->modified_indices = (int *) realloc(bktr_state->modified_indices,sizeof(int));
						bktr_state->num_modified_indices = 1;
						bktr_state->curr_value_index++;
						*(bktr_state->modified_indices) = bktr_state->row*9+bktr_state->col;
						matrix[bktr_state->row][bktr_state->col] = *(bktr_state->possible_values + bktr_state->curr_value_index);
						
						remaining--;
								
					}
					free(value_array);
				}
			}
			if(one_value_flag)
			{
				break;
			}
		}
	}
	
	printf("number of iterations : %d\n",count);
}

int possible_values(int *mat,int row, int col,int *value_array)
{
	int return_value = 0;
	int i;
	
	for(i = 1; i <= 9; i++)
	{
		if(check_in_row_col_square(mat,row,col,i))
		{
			continue;
		}
			
		value_array = (int *) realloc(value_array,sizeof(int)*(return_value+1));
		*(value_array+return_value++) = i;
		
	}
	
	return return_value;
}

int check_in_row_col_square(int *mat,int row,int col,int num)
{
	int i,j,cur_row_range,cur_col_range;
	
	for(i = 0; i < 9; i++)
	{
		if(*(mat+row*9+i) == num)
		{
			return 1;
		}
	}
	
	for(i = 0; i < 9; i++)
	{
		if(*(mat+i*9+col) == num)
		{
			return 1;
		}
	}
	
	cur_row_range = (row/3)*3;
	cur_col_range = (col/3)*3;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(*(mat+(cur_row_range+i)*9+cur_col_range+j) == num)
			{
				return 1;
			}
		}
	}
	
	return 0;
}

void push(struct backtracking_state *bktr_state)
{
	backtracking_stack = (struct backtracking_state **) realloc( backtracking_stack, sizeof(struct backtracking_state *) * (backtracking_stack_ptr+1));
	*(backtracking_stack+backtracking_stack_ptr++) = bktr_state; 
}

void pop()
{
	struct backtracking_state *bktr_state;
	bktr_state = *(backtracking_stack+backtracking_stack_ptr-1);
	free(bktr_state->possible_values);
	free(bktr_state->modified_indices);
	free(bktr_state);
	backtracking_stack_ptr--;
}

