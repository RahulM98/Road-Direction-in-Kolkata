#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
struct edge
{
	char location[27];
	float distance;
	char road_name[60];
	struct edge *next;
};
struct vertex
{
	char location[27];
	struct edge *edge_address;
	int status;
	float distance;
	char prev_source[27];
	struct vertex *next;
};
struct vertex *root = NULL;
int alpha = 5;
int cursor_pos = 0;
int time = 0;
void build_graph_from_FILE(char array[],int size);
int no_of_edges(char arr[]);
char word_list[90];
float know_distance()                       //  From FILE
{
	int i = 0,l,decimal,num = 0;
	char arr[10];
	char s,x;
	float d=0.0;
	FILE *fp;
	fp = fopen("Dijkstra.txt","r");
	fseek(fp,cursor_pos,0);
	s = fgetc(fp);
	while((s != '~') && (s != EOF) && (s != '\n'))
	{
		arr[i] = s;
		i++;
		s = fgetc(fp);
	}
	if(i != 9)
		arr[i] = '\0';
	sscanf(arr,"$%f",&d);
	cursor_pos = ftell(fp);
	fclose(fp);
	return d;
}
struct edge *build_edge_list(char loc1[])                    // For the vertices it makes their respective edges list
{                                                            // Basically making the horizontal linked list
	int n,i=1,l,count=0;
	float dist;
	char loc2[27],road[60],arr[10];
	char s;                                               /////////////////////////////////
	struct edge *start = NULL,*last = NULL;
	struct edge *temp;
	if(alpha != 5)
	{
		printf("Enter no of locations connected to %s :",loc1);
		scanf("%d",&n);
	}
	else
		n = no_of_edges(loc1);
	while(n>0 && i<=n)
	{
		if(alpha != 5)
		{
			if(i == 1)
				printf("Enter the %dst location connected to %s :",i,loc1);
			else if(i == 2)
				printf("Enter the %dnd location connected to %s :",i,loc1);
			else if(i == 3)
				printf("Enter the %drd location connected to %s :",i,loc1);
			else
				printf("Enter the %dth location connected to %s :",i,loc1);
			scanf(" %s",&s);                                                             ///////////  ?????????  ///////////
			printf("Enter the distance between %s and %s :",loc1,loc2);
			scanf("%f",&dist);
			printf("Enter the name of the road between %s and %s",loc1,loc2);
			gets(road);                                                                   ////////////  ?????????  ///////////
		}
		else
		{
			count++;
			build_graph_from_FILE(loc2,27);
			build_graph_from_FILE(road,50);
			dist = know_distance();
		}
		temp = (struct edge *)malloc(sizeof(struct edge));
		strcpy(temp->location,loc2);                         ///  temp->location = s;
		strcpy(temp->road_name,road);
		temp->distance = dist;                               ///  temp->weight = wt;
		temp->next = NULL;
		if(start == NULL)
			start = temp;
		else
			last->next = temp;
		last = temp;
		i++;
	}
	return start;
}
struct vertex *build_graph(struct vertex *last,char loc[])             // From the picked up vertices, it makes the vertical list of vertices 
{
	struct vertex *temp;
	temp = (struct vertex *)malloc(sizeof(struct vertex));
	strcpy(temp->location,loc);                                      ///  temp->location = alphabet+65;
	temp->edge_address = build_edge_list(loc);
	temp->next = NULL;
	if(root == NULL)
		root = temp;
	else
		last->next = temp;
	last = temp;
	return last;
}
/*void display_graph(int n)                                                    //////////////  ???????????  ///////////////
{
	int i;
	struct vertex *temp = root;
	struct edge *node;
	printf("location\tEdges\tRoad\n\n");
	while(temp != NULL)                       //for(i=1;i<=n;i++)
	{
		printf("%s      ",temp->location);
		node = temp->edge_address;
		while(node != NULL)
		{
			printf("%s  (%f)     %s",node->location,node->distance,node->road_name);
			node = node->next;
		}
		temp = temp->next;
		printf("\n");
	}
}*/
int search(struct vertex *root,char array[])                                      // Searching of a location is done with the help of the vertical Linked List
{
	if(root == NULL)
		return 0;                                                                 // Location is not found  i.e. returns 0
	else if(strcmp(root->location,array) == 0)
		return 1;                                                                 // Location is Found i.e. returns 1
	else
		return search(root->next,array);
}
struct vertex *find_min()
{
	struct vertex *minimum,*temp;
	temp = root;
	while(temp != NULL)
	{
		if(temp->status != 2)
			minimum = temp;
		temp = temp->next;
	}
	temp = root;
	while(temp != NULL)                       //for(i=1;i<=no_of_node;i++)
	{
		if((temp->status != 2) && (temp->distance < minimum->distance))
			minimum = temp;
		temp = temp->next;
	}
	return minimum;
}
struct vertex *send_address_V(char name_of_place[])                            /////    sends the vertex address from vertical list
{
	struct vertex *temp = root;                                               // Previous Place Address
	while(temp != NULL)
	{
		if(strcmp(temp->location,name_of_place) == 0)                          ///  if(temp->location == name_of_place)
			return temp;
		temp = temp->next;
	}
}
struct edge *send_address_H(struct vertex *from,struct vertex *to)
{
	struct edge *node;
	node = to->edge_address;
	while(node != NULL)
	{
		if(strcmp(from->location,node->location) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}
int know_direction(char from_pos[],char curr_pos[],char to_pos[])
{
	int i,dir;
	bool x = false;
	char arr1[20],arr2[20],arr3[20],direction[10];
	FILE *fp;
	fp = fopen("DIRECTION.txt","r");
	char s;
	s = fgetc(fp);
	while(s != EOF)
	{
		if(s == '\n')
		{
			i = 0;
			s = fgetc(fp);
			while(s != '~')
			{
				arr1[i] = s;
				i++;
				s = fgetc(fp);
			}
			arr1[i] = '\0';
			if(strcmp(arr1,curr_pos) == 0)   //  found B
				break;
		}
		s = fgetc(fp);
	}
	while(s != '\n')
	{
		if(s == '~')
		{
				i = 0;
				s = fgetc(fp);
			while(s != '|')
			{
				arr2[i] = s;
				i++;
				s = fgetc(fp);
			}
			arr2[i] = '\0';
			if(strcmp(arr2,to_pos) == 0)      //  found A
			{
				while((s != '~') && (s != '\n'))
				{
					if(s == '|')
					{
						i = 0;
						s = fgetc(fp);
						while(s != '@')
						{
							arr3[i] = s;
							i++;
							s = fgetc(fp);
						}
						arr3[i] = '\0';
						if(strcmp(arr3,from_pos) == 0)
						{
							i = 0;
							s = fgetc(fp);
							while((s != '~') && (s != '\n'))
							{
								direction[i] = s;
								i++;
								s = fgetc(fp);
							}
							direction[i] = '\0';
							x = true;
						}
						else
							s = fgetc(fp);
					}
					else
						s = fgetc(fp);
				}
			}
		}
		else
			s = fgetc(fp);
		if(x == true)
		{
			x = false;
			break;
		}
	}
	fclose(fp);
	sscanf(direction,"@%d",&dir);
	return dir;
}
void Dijkstra_path(char source[],char destination[],int no_of_node)            // The Dijkstra alogrithm
{
	int i,j;
	float D;
	struct edge *address;
	struct vertex *temp = root;
	struct vertex *min,*temp2,*temp1;
	for(i=1;i<=no_of_node;i++)
	{
		temp->status = 1;
		if(strcmp(temp->location,source) == 0)
		{
			temp->distance = 0;
			strcpy(temp->prev_source,"NULL");
		}
		else
		{
			temp->distance = INFINITY;
			strcpy(temp->prev_source,source);
		}
		temp = temp->next;
	}
	for(i=1;i<=no_of_node;i++)
	{
		min = find_min();
		min->status = 2;
		if(strcmp(min->location,destination) == 0)
			break;
		temp2 = root;
		for(j=1;j<=no_of_node;j++)
		{
			if(temp2->status != 2)
			{
				address = send_address_H(min,temp2);
				if(address == NULL)
					D = INFINITY;
				else
					D = address->distance;
				if(temp2->distance > (min->distance + D))
				{
					temp2->distance = min->distance + D;
					strcpy(temp2->prev_source,min->location);
				}
			}
			temp2 = temp2->next;
		}
	}
}
void display_path(char source[],char destination[],int no_of_node)
{
	int i,j,x;
	char from_pos[30],curr_pos[30],to_pos[30];
	char name[27];
	strcpy(name,destination);
	struct vertex *path[no_of_node];
	struct vertex *temp;
	struct edge *temporary;
	for(i=0;i<no_of_node;i++)
	{
		temp = send_address_V(name);
		path[i] = temp;
		if(strcmp(temp->location,source) == 0)
			break;
		strcpy(name,temp->prev_source);
	}
	printf("\n\n\n");
	for(j=i;j>=0;j--)
	{
		if(j == i)
			printf("Board on your car.");
		else
			printf(" %s.",path[j]->location);
		if(j != 0)
		{
			temporary = send_address_H(path[j],path[j-1]);
			if(j == i)
			{
				printf(" Then from %s go straight onto %s, drive %.2f KM and reach",path[j]->location,temporary->road_name,temporary->distance);
			}
			else
			{
				strcpy(from_pos,path[j+1]->location);
				strcpy(curr_pos,path[j]->location);
				strcpy(to_pos,path[j-1]->location);
				x = know_direction(from_pos,curr_pos,to_pos);
				if(x == 1)
					printf(" Then from %s turn left onto %s, drive %.2f KM and reach",path[j]->location,temporary->road_name,temporary->distance);
				else if(x == 2)
					printf(" Then from %s turn right onto %s, drive %.2f KM and reach",path[j]->location,temporary->road_name,temporary->distance);
				else
					printf(" Then from %s go straight onto %s, drive %.2f KM and reach",path[j]->location,temporary->road_name,temporary->distance);
			}
			//temporary = send_address_H(path[j],path[j-1]);
			//printf(" Then from %s take %s and go %.2f KM and reach",path[j]->location,temporary->road_name,temporary->distance);
		}
	}
	printf("\n");
}
void build_graph_from_FILE(char array[],int size)                           // Reads the names of locations (vertex) from FILE
{
	char s;
	int i = 0;
	struct vertex *last = NULL;
	FILE *fp;
	fp = fopen("Dijkstra.txt","r");
	fseek(fp,cursor_pos-1,0);
	s = fgetc(fp);
	while(s != EOF)
	{
		if(s == '\n')
		{
			i = 0;
			s = fgetc(fp);
			while(/*(s != EOF) && */(s != '~')/* && (s != '@') && (s != '$')*/)
			{
				array[i] = s;
				i++;
				s = fgetc(fp);
			}
			array[i] = '\0';
			break;
		}
		else if(s == '~')
		{
			i = 0;
			s = fgetc(fp);
			while((s != EOF) && (s != '\n') && (s != '@') && (s != '$'))
			{
				array[i] = s;
				i++;
				s = fgetc(fp);
			}
			array[i] = '\0';
			break;
		}
		else if(s== '@')
		{
			i = 0;
			s = fgetc(fp);
			while((s != EOF) && (s != '\n') && (s != '~') && (s != '$'))
			{
				array[i] = s;
				i++;
				s = fgetc(fp);
			}
			array[i] = '\0';
			break;
		}
		else if(s == '$')
		{
			i = 0;
			s = fgetc(fp);
			while((s != EOF) && (s != '\n') && (s != '~') && (s != '@'))
			{
				array[i] = s;
				i++;
				s = fgetc(fp);
			}
			array[i] = '\0';
			break;
		}
	}
	cursor_pos = ftell(fp);
	fclose(fp);
}
int lineCount_FILE()                                         // Returns the total no of line in the File
{
	int n=0;
	char s;
	FILE *fp;
	fp = fopen("Dijkstra.txt","r");
	while((s=fgetc(fp)) != EOF)
	{
		if(s == '\n')
			n++;
	}
	fclose(fp);
	return n;
}
void FILE_input(FILE *fp,char array[],int size)                 //   taking input from FILE
{                                                               //   Taking only 1st word (string) from each line
	char s;
	int i = 0;
	s = fgetc(fp);
	while((s != '~') && (s != EOF) && (i != size))
	{
		array[i] = s;
		i++;
		s = fgetc(fp);
	}
	if(i != size)
		array[i] = '\0';
}
int no_of_edges(char arr[])
{
	int n = 0;
	char s,string[27];
	FILE *fp;
	fp = fopen("Dijkstra.txt","r");
	while((s=fgetc(fp)) != EOF)
	{
		if(s == '\n')
		{
			FILE_input(fp,string,27);
			if((strcmp(string,arr)) == 0)
			{
				s = fgetc(fp);
				while((s != '\n') && (s != EOF))
				{
					if(s == '@')
						n++;
					s = fgetc(fp);
				}
				break;
			}
		}
	}
	fclose(fp);
	return n;
}
void send_words_from_FILE(int n,char word[])                                   // Sends the similar words from the FILE where words are arranged
{                                                                              // Basically for word suggestion purpose
	int i = 0,len,c,count = 0,max_word_count = 0;
	char s;
	struct vertex *temp;
	char new_word[27];
	strupr(word);
	temp = root;
	word_list[0] = '\0';
	while(temp != NULL)
	{
		strcpy(new_word,temp->location);
		c = 0;
		for(i=0;i<n;i++)
		{
			if(word[i] == new_word[i])
				c++;
		}
		if(c == n)
		{
			max_word_count = max_word_count + 1;
			len = strlen(new_word);
			for(i=0;i<len;i++)
			{
				word_list[count] = new_word[i];
				count++;
			}
			word_list[count] = ',';
			count++;
		}
		if((max_word_count == 2) || (count == 89))                                                // Sends maximum 3 similar words
			break;
		temp = temp->next;
	}
	word_list[count-1] = '\0';
}
void user_input(char array[],int size,char array_user[])                      // Takes the user input without pressing of Enter key
{
	int ch,i;
	for(i=0;i<size;i++)
	{
		ch = getch();
		printf("\r                                                                               ");
		if(ch == 8)                                                             // For Backspace key
		{
			array[i-1] = '\0';
			array_user[i-1] = '\0';
			send_words_from_FILE(i-1,array);
			if(time == 1)
				printf("\rYour current location :%s         %s",array_user,word_list);
			else
				printf("\rYour destination :%s         %s",array_user,word_list);
			i = i-2;
		}
		else if(ch == 13)                                                      // For Enter key
		{
			if(isspace(array[i-1]))
			{
				array[i-1] = '\0';
				array_user[i-1] = '\0';
			}
			else
			{
				array[i] = '\0';
				array_user[i] = '\0';
			}
			if(time == 1)
				printf("\rYour current location :%s",array_user);
			else
				printf("\rYour destination :%s",array_user);
			break;
		}
		else if(ch == 32)                                                      // For Space Bar key
		{
			if(i==0 || isspace(array[i-1]))
				i = i-1;
			else
			{
				array[i] = ch;
				array_user[i] = ch;
				array[i+1] = '\0';
				array_user[i+1] = '\0';
			}
			if(time == 1)
				printf("\rYour current location :%s         %s",array_user,word_list);
			else
				printf("\rYour destination :%s         %s",array_user,word_list);
		}
		else                                                                   // For a valid character
		{
			array[i] = ch;
			array_user[i] = ch;
			array[i+1] = '\0';
			array_user[i+1] = '\0';
			send_words_from_FILE(i+1,array);
			if(time == 1)
				printf("\rYour current location :%s         %s",array_user,word_list);
			else
				printf("\rYour destination :%s         %s",array_user,word_list);
		}
		if(i < 0)
		{
			i = -1;
			printf("\r                                                                               ");
			if(time == 1)
				printf("\rYour current location :");
			else
				printf("\rYour destination :");
		}
	}
}
/*int password()
{
	int i,j,ch;
	char password[12];
	printf("Enter the password (maximum 10 characters) : ");
	for(i=0;i<10;i++)
	{
		ch = getch();
		if(ch == 8)
		{
			i = i-2;
			printf("\r                                                                               ");
			printf("\rEnter the password (maximum 10 characters) : ");
			for(j=0;j<=i;j++)
				printf("*");
		}
		else if(ch == 13)
			break;
		else
		{
			password[i] = ch;
			printf("*");
		}
		if(i < 0)
		{
			printf("\r                                                                               ");
			printf("\rEnter the password (maximum 10 characters) : ");
			i = -1;
		}
	}
	password[i] = '\0';
	if((strcmp(password,"Rahul...")) == 0)
		return 1;
	else
		return 0;
}*/
void load_map()
{
	int n,i;
	char location_name[27];
	struct vertex *last = NULL;
	n = lineCount_FILE();
	for(i=1;i<=n;i++)
	{
		FILE *fp;
		fp = fopen("Dijkstra.txt","r");
		build_graph_from_FILE(location_name,27);
		last = build_graph(last,location_name);
		fclose(fp);
	}
}
main()
{
	int ch,i,n;
	char city_name[50],source[27],destination[27],src_user[27],dest_user[27];
	//char location_name[27];
	struct vertex *temp;
	//struct vertex *last = NULL;
	load_map();
	strcpy(city_name,"KOLKATA");
	printf("\n\t\t\t\tThe SHORTEST PATH\n\t\t\t\t\t--- By Rahul Manna\n\n");
	printf("\t----------------------------------------------------------------\n");
	printf("\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
	printf("\t----------------------------------------------------------------\n\n");
	printf("Welcome to the program \" The SHORTEST PATH \"");
	printf("\nPlease let me know, what do you want to do with me by pressing the numbers from the MENU...\n");
	while(ch != '2')
	{
		printf("\n\n\t-------- Program MENU --------\n\n");
		printf("Press   1 To see the shortest path betweeen two locations in the city %s",city_name);
		//printf("\n\t2 To use this program for a different city\n\t2 To Exit\n\n\n");
		printf("\n\t2 To Exit\n\n\n");
		printf("Enter your choice : ");
		fflush(stdin);
		ch = getchar();
		//printf("AA\n");
		switch(ch)
		{
			case '1':
				//printf("AA\n");
				n = lineCount_FILE();
				/*for(i=1;i<=n;i++)
				{
					FILE *fp;
					fp = fopen("Dijkstra.txt","r");
					build_graph_from_FILE(location_name,27);
					last = build_graph(last,location_name);
					fclose(fp);
				}*/
				//printf("AA\n");
				time = 1;
				printf("\nYour current location :");
				user_input(source,27,src_user);
				while(search(root,source) != 1)
				{
					printf("\n\nEntered location is not found.Please re-enter your current location.");
					printf("\nYour current location :");
					user_input(source,27,src_user);
				}
				time = 2;
				printf("\nYour destination :");
				user_input(destination,27,dest_user);
				while(search(root,destination) != 1)
				{
					printf("\n\nEntered location is not found.Please re-enter your destination.");
					printf("\nYour destination :");
					user_input(destination,27,dest_user);
				}
				time = 0;
				//printf("AA\n");
				Dijkstra_path(source,destination,n);
				//printf("AA\n");
				display_path(source,destination,n);
				break;
			/*case '2':
				if(password() == 1)
				{
					printf("\n\tAccess granted...\n\n");
					printf("Enter the name of the new city : ");
					fflush(stdin);
					gets(city_name);
					alpha = -5;
					printf("Enter no of location in the City :");
					scanf("%d",&n);
					for(i=1;i<=n;i++)
					{
						printf("Enter the location :-");
						scanf("%s",location_name);                                   ////////////  ??????????  //////////////
						last = build_graph(last,location_name);
					}
				}
				else
					printf("\n\tIncorrect password.\n\tAccess denied...");
				alpha = 5;
				break;*/
			case '2':
				printf("\nThanks for visiting...\n\tPlease visit again...");
				break;
			default :
				printf("Please press any of the numbers from the MENU\n\n\n");
		}
		printf("\n\n\n");
	}
}
