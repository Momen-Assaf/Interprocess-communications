#include "local.h"

int main(int argc, char** argv) {
    if(argc < 3){
    perror("No arguments passed, Please enter the number of Helper,Spy processes.");
    exit(0);
    }
    printf("%d,%d\n",atoi(argv[1]),atoi(argv[2]));
    // calculate the number of children
    int n = 3 + atoi(argv[1]) + atoi(argv[2]);
    printf("Iam parent proccess\nnumber of children: %d\n", n); 
    pid_t parent_id = getpid();
    pid_t children_id[n];
    for (int i = 0; i < n; i++){
        children_id[i] = fork();

        if(children_id[i] == -1){
            perror("No Memory\n");
            exit(-1);
        }
        else if (children_id[i] == 0){
            if( i < (atoi(argv[1]))){
                //the user_defined number of children are the helper proccess
                execlp("./helper","helper",NULL);
            }
            else if(i == atoi(argv[1]) + 1){
                //the sender process child
                execlp("./sender","sender",NULL);
            }
            else if(i == atoi(argv[1]) + 2){
                //the receiver process child
                execlp("./receiver","receiver",NULL);
            }
            else if(i == atoi(argv[1]) + 3){
                //the master_spy process child
                execlp("./master_spy","master_spy",NULL);
            }
            else{ 
                //the rest are the spy processes
                execlp("./spy","spy",NULL);
            }
            //children shouldnt run this code
            perror("Exec Error");
            exit(-1);
            break;
        }
        else{
            //printf("The Parent procces with id %d created the child with id: %d\n",parent_id,children_id[i]);
        }
    }
    sleep(1);
    for (int k = 0 ; k < n ; k++){
        kill(children_id[k],SIGTERM);
        printf("Pid:%d killed\n",children_id[k]);
    }

}