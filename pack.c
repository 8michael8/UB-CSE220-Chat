#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "serialize.h"


/* Pack the user input provided in input into the appropriate message
 * type in the space provided by packed.  You can assume that input is a
 * NUL-terminated string, and that packed is a buffer of size
 * PACKET_SIZE.
 *
 * Returns the packet type for valid input, or -1 for invalid input.
 */
int pack(void *packed, char *input) {
    char ubit[] = "mleung8";
    if(input[0] == '/' && input[1] == 'm' && input[2] == 'e'){
        int count = 0;
        if(input[3] != ' '){
            return -1;
        }else{
            *(int *)packed = STATUS;
            packed += sizeof(int);
            for(int i = 0; i < 16;i++){
        if(i < 8){
            *(char *)packed = ubit[i];
            packed++;
        }else{
            *(char *)packed = '\0';
            packed++;
            }
        }
            for(int i = 3;input[i] != '\0';i++){
                if(input[i] != ' '){
                    for(int j = i; input[j] != '\0';j++){
                        count++;
                    }
                    break;
                }
            }
            if(count == 0){
                return -1;
            }
            if (count > MAX_MESSAGE_SIZE){
            return -1;
        }
            *(size_t *)packed = count;
            packed += sizeof(size_t);
            *(size_t *)packed = 0;
            packed += sizeof(size_t);

            for(int i = 3; input[i] != '\0';i++){
                if(input[i] != ' '){
                    for(int j = i; input[j] != '\0';j++){
                        *(char *)packed = input[j];
                        packed++;
                    }
                    break;
                }
            }
            // *(char *)packed = '\0';
            return STATUS;
            
            
        }
    }else if(input[0] == '@'){
        if(input[1] == ' '){
            return -1;
        }else{
            int count = 0;
            int placeholder = 0;
            *(int *)packed = LABELED;
            packed += sizeof(int);
            for(int i = 0; i < 16;i++){
        if(i < 8){
            *(char *)packed = ubit[i];
            packed++;
        }else{
            *(char *)packed = '\0';
            packed++;
            }
        }
            for(int i = 0; input[i] != '\0';i++){
                if(input[i] == ' '){
                    for(int j = i +1;input[j] != '\0';j++){
                        if(input[j] != ' '){
                            placeholder = j;
                            break;
                        }
                    }
                    break;
                }
            }
            if(placeholder == 0){
                return -1;
            }
            for(int k = placeholder;input[k] != '\0';k++){
                count++;
            }
            if(count > MAX_MESSAGE_SIZE ){
                return -1;
            }
            *(size_t *)packed = count;
            packed += sizeof(size_t);
            count = 0;
            
            for(int i = 1; input[i] != ' ';i++){
                count++;
            }
            if(count > NAME_SIZE){
                return -1;
            }
            *(size_t *)packed = count;
            packed += sizeof(size_t);

            *(size_t *)packed = 0;
            packed += sizeof(size_t);

            for(int i = placeholder; input[i] != '\0';i++){
                *(char *)packed = input[i];
                packed++;
            }/*
             *(char *)packed = '\0';
             packed++;*/

             for(int i = 1; input[i] != ' ';i++){
                 *(char *)packed = input[i];
                 packed++;
            }

             //*(char *)packed = '\0';
             return LABELED;

            
        }
        
        
    }else if(input[0] == '/' && input[1] == 's' && input[2] == 't'&& input[3] == 'a'&& input[4] == 't'&& input[5] == 's'){
        if(input[6] != '\0'){
            return -1;
        }else{
            *(int *)packed = STATISTICS;
            packed += sizeof(int);
            //ubit
            for(int i = 0; i < 16;i++){
        if(i < 8){
            *(char *)packed = ubit[i];
            packed++;
        }else{
            *(char *)packed = '\0';
            packed++;
            }
        }
            //*(char *) packed = '\0';

            return STATISTICS;
            
            
        }
    }else{
        bool x = false;
        int count = 0;
        *(int *) packed = MESSAGE;
        packed += sizeof(int);
        for(int i = 0; i < 16;i++){
        if(i < 8){
            *(char *)packed = ubit[i];
            packed++;
        }else{
            *(char *)packed = '\0';
            packed++;
            }
        }
        for(int i = 0; input[i] != '\0';i++){
            if(input[i] != ' '){
                x = true;
            }
            count++;
        }
        if(x == false){
            return -1;
        }
        if (count > MAX_MESSAGE_SIZE){
            return -1;
        }
        *(size_t *)packed = count;
        packed += sizeof(size_t);
        *(size_t *)packed = 0;
        packed+=sizeof(size_t);
        for(int i = 0; i < count; i ++){
            *(char *)packed = input[i];
            packed++;
        }
        //*(char *)packed = '\0';
        return MESSAGE;
    }
    return -1;
}

/* Create a refresh packet for the given message ID.  You can assume
 * that packed is a buffer of size PACKET_SIZE.
 *
 * You should start by implementing this method!
 *
 * Returns the packet type.
 */
int pack_refresh(void *packed, int message_id) {
    char ubit[] = "mleung8";
    *(int *) packed = REFRESH;
    packed+= sizeof(int);
    //memcpy(packed,ubit,7);
    //packed += 7;
    for(int i = 0; i < 16;i++){
        if(i < 8){
            *(char *)packed = ubit[i];
            packed++;
        }else{
            *(char *)packed = '\0';
            packed++;
        }
    }
    *(int *)packed = message_id;
    return REFRESH;
}
