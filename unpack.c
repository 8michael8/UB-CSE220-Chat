#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "serialize.h"

/* Unpack the given packet into the buffer message.  You can assume
 * that packed points to a packet buffer large enough to hold the
 * packet described therein, but you cannot assume that the packet is
 * otherwise valid.  You can assume that message points to a character
 * buffer large enough to store the unpacked packet, if it is a valid
 * packet.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */

int unpack(char *message, void *packed) {
    int type = *(int *)packed;
    memset(message,'\0',strlen(message));
    int placeholder = 0;
    bool x = false;
    if(type == MESSAGE){
        int count = 0;
        packed += sizeof(int);
        while(*(char *)packed != '\0'){
            message[count] = *(char *)packed;
            packed++;
            count++;
        }
        if(count > NAME_SIZE){
            return -1;
        }
        packed += 16-count;
        message[count] = ':';
        count++;
        message[count] = ' ';
        count++;
        placeholder = count;
        
        int size = *(size_t*)packed;
        if(size >MAX_MESSAGE_SIZE){
            return -1;
        }
        
        packed += sizeof(size_t);
        packed += sizeof(size_t);
        for(int i =0; i< size;i++){
            message[count] = *(char *) packed;
            if(message[count] != ' '){
                x = true;
            }
            packed++;
            count++;
        }
        if(count == placeholder){
            return -1;
        }
        message[count] = '\0';
        if(x == false){
            return -1;
        }
        return MESSAGE;
    }else if(type == STATUS){
        int count = 0;
        packed += sizeof(int);
        while(*(char *)packed != '\0'){
            message[count] = *(char *)packed;
            packed++;
            count++;
        }
        if(count > NAME_SIZE){
            return -1;
        }
        packed += 16-count;
        message[count] = ' ';
        count++;
        int size = *(size_t*)packed;
        if(size >MAX_MESSAGE_SIZE){
            return -1;
        }
        packed += 2 * sizeof(size_t);
        for(int i = 0; i < size;i++){
            message[count] = *(char *)packed;
            packed++;
            count++;
        }
        message[count] = '\0';
        return STATUS;
    }else if(type == LABELED){
        int messageCount = 0;
        int targetCount = 0;
        int count = 0;
        packed += sizeof(int);
        while(*(char *)packed != '\0'){
            message[count] = *(char *)packed;
            packed++;
            count++;
        }
        if(count > NAME_SIZE){
            return -1;
        }
        packed += 16-count;
        message[count] = ':';
        count++;
        message[count] = ' ';
        count++;
        messageCount = *(size_t *)packed;
        if(messageCount >MAX_MESSAGE_SIZE){
            return -1;
        }
        packed += sizeof(size_t);
        targetCount = *(size_t *)packed;
        packed += 2 * sizeof(size_t);
        packed += messageCount;
        message[count] = '@';
        count++;
        for(int i = 0; i< targetCount;i++){
            message[count] = *(char *)packed;
            packed++;
            count++;
        }
        message[count] = ' ';
        count++;
        packed -= (messageCount + targetCount);
        for(int i = 0;i<messageCount;i++){
            message[count] = *(char *)packed;
            packed++;
            count++;
        }

        message[count] = '\0';
        return LABELED;
        
    }else{
        return -1;
    }
    return -1;
}

/* Unpack the given packed packet into the given statistics structure.
 * You can assume that packed points to a packet buffer large enough to
 * hold the statistics packet, but you cannot assume that it is
 * otherwise valid.  You can assume that statistics points to a
 * statistics structure.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack_statistics(struct statistics *statistics, void *packed) {
    packed += sizeof(int);
    int count = 0;
    while(*(char*)packed != '\0'){
        if(count == NAME_SIZE){
            break;
        }
        statistics->sender[count]=*(char*)packed;
        packed++;
        count++;
    }
    if(count > NAME_SIZE){
        return -1;
    }
    for(int i = count; i < NAME_SIZE + 1;i++){
        statistics->sender[i] = '\0';
        packed++;
    }
    packed--;
    count = 0;
    while(*(char*)packed != '\0'){
        if(count == NAME_SIZE){
            break;
        }
        statistics->most_active[count] = *(char*)packed;
        packed++;
        count++;
    }
    if(count > 16){
        return -1;
    }
    for(int i = count; i < NAME_SIZE + 1;i++){
        statistics->most_active[i] = '\0';
        packed++;
    }
    packed--;
    statistics -> most_active_count = *(int *)packed;
    packed += sizeof(int);

    statistics -> invalid_count = *(long *)packed;
    packed += sizeof(long);

    statistics -> refresh_count = *(long *)packed;
    packed += sizeof(long);

    statistics -> messages_count = *(int *)packed;
    packed += sizeof(int);
    
    return STATISTICS;
}
