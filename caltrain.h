#include <pthread.h>

struct station {
	// FILL ME IN
    int availableSeats;
    int passengersStationNumber;
    int passengerInTrain;
    pthread_mutex_t passengersNumberMutex;
    pthread_cond_t trainArrival;
    pthread_cond_t passengerArrival;
    pthread_cond_t passengerOnBoard;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
