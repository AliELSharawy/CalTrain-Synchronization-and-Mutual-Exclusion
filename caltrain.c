#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "caltrain.h"



void station_init(struct station *station)
{
    // FILL ME IN
    pthread_mutex_init(&station -> passengersNumberMutex,NULL);
    pthread_cond_init(&station -> trainArrival,NULL);
    pthread_cond_init(&station -> passengerOnBoard,NULL);
    pthread_cond_init(&station ->passengerArrival,NULL);
    station -> availableSeats = 0;
    station -> passengersStationNumber = 0;
    station -> passengerInTrain = 0;
}

void station_load_train(struct station *station, int count){
    // FILL ME IN

    // check if train has available seats and there are passengers waiting for it,
    // so we return immediately when no waiting passengers or no available seats
    if(count == 0 || station->passengersStationNumber == 0)
        return;

    station -> availableSeats = count;

    pthread_mutex_lock(&station -> passengersNumberMutex);

    // we lock here to make load train able to wait after broadcasting passengers
    // as any new passenger can acquire lock before load train so this will cause train to wait till re-acquire lock
    pthread_cond_broadcast(&station -> trainArrival);
    // we wait as long as there passengers able to ride the train but not in train yet
    // so train does not return early without passengers ride
    while(station->availableSeats > 0 && station->passengersStationNumber > 0) {
        pthread_cond_wait(&station->passengerArrival, &station->passengersNumberMutex);
    }
    // when all passengers in train ,so we wait for signal from on board to check tickets all passenger are in train to leave
    if(station->passengerInTrain > 0)
        pthread_cond_wait(&station->passengerOnBoard, &station->passengersNumberMutex);
    pthread_mutex_unlock(&station -> passengersNumberMutex);

}

void station_wait_for_train(struct station *station)
{
    // FILL ME IN
    pthread_mutex_lock(&station -> passengersNumberMutex);

    station -> passengersStationNumber++;
    // here we wait till a train arrive and adjust available seats
    // passenger will be able to enter train if there is available seats else it will wait for the next train to come
    while(station -> availableSeats == 0){
        pthread_cond_wait(&station -> trainArrival,&station -> passengersNumberMutex);
    }

    // passenger will ride train so remove this passenger form waiting passengers in station
    // and decrease available seats and increase number passengers in train.
    station->passengerInTrain++;
    station -> passengersStationNumber--;
    station -> availableSeats--;
    pthread_mutex_unlock(&station -> passengersNumberMutex);
    // then signal load train that a passenger ride the train
    pthread_cond_signal(&station->passengerArrival);

}

void station_on_board(struct station *station)
{
    // FILL ME IN
    // this function is called after train come and broadcast all passengers
    // so passengers who will ride train and leave are awakened and other sleep and wait for next train
    // so this function is called after passenger do its task so no need to lock passenger in train
    // also this function is called a time not many threads in same time
    // we count al passengers in train like checking tickets and when all passengers in train we signal train to leave
    station->passengerInTrain--;
    if(station ->passengerInTrain == 0) {
        pthread_cond_signal(&station->passengerOnBoard);
    }
}