#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUM 18

int memory = 0;

int readcount = 0;   // Đếm số lượng người đọc
// Tất cả semaphore sẽ được khởi tạo với giá trị 1
sem_t resource;      // Kiểm soát truy cập vào tài nguyên
sem_t mutex;         // Đồng bộ hoá các thay đổi của readcount
sem_t serviceQueue;  // CÔNG BẰNG: duy trì thứ tự các yêu cầu
                     // (theo nguyên tắc FIFO)
void* reader(void* arg) {
    sem_wait(&serviceQueue); // Xếp hàng đợi để được truy cập vào tài nguyên
    sem_wait(&mutex);        // Yêu cầu truy cập giá trị readcount
    readcount++;             // Cộng thêm 1 người đọc vào readcount
    if (readcount == 1)      // Nếu là người đọc đầu tiên yêu cầu tài nguyên
        sem_wait(&resource); // cấp quyền cho người đọc (người viết bị chặn)
    sem_post(&serviceQueue); // Thoát khỏi hàng đợi, cho người tiếp theo
    sem_post(&mutex);        // Giải phóng quyền truy cập vào readcount

    sleep(rand() % 3 + 1);
    printf("\033[1;31mReader %2d\033[1;0m: read as %d\n", (int)arg, memory);

    sem_wait(&mutex);        // Yêu cầu truy cập giá trị readcount
    readcount--;             // Loại bỏ 1 người đọc trong readcount 
    if (readcount == 0)      // Nếu không còn người đọc
        sem_post(&resource); // giải phóng quyền truy cập tài nguyên cho tất cả
    sem_post(&mutex);        // Giải phóng quyền truy cập vào readcount
}
void* writer(void* arg) {
    sem_wait(&serviceQueue); // Xếp hàng đợi để được truy cập vào tài nguyên
    sem_wait(&resource);     // Yêu cầu quyền truy cập vào tài nguyên
    sem_post(&serviceQueue); // Thoát khỏi hàng đợi, cho người tiếp theo

    memory = rand() % 90 + 10;
    printf("\033[1;32mWriter %2d\033[1;0m: modified to %d\n", (int)arg, memory);
    sleep(rand() % 3 + 2);

    sem_post(&resource); // Giải phóng quyền truy cập tài nguyên cho người tiếp theo
}

int main() {
    pthread_t th[THREAD_NUM];
    sem_init(&serviceQueue, 0, 1);
    sem_init(&resource, 0, 1);
    sem_init(&mutex, 0, 1);

    srand(time(NULL));
    int idx_writer[THREAD_NUM] = {};
    int cnt_w, num_w = round(THREAD_NUM * (1.0 / 3.0));

    while (cnt_w != num_w) {
        idx_writer[rand() % THREAD_NUM] = 1;
        cnt_w = 0;
        for (int i = 0; i < THREAD_NUM; ++i)
            if (idx_writer[i] == 1) cnt_w++;
    }
    puts("READERS WRITERS");
    for (int i = 0; i < THREAD_NUM; i++) {
        if (idx_writer[i])
            pthread_create(&th[i], NULL, &writer, (void*)i);
        else
            pthread_create(&th[i], NULL, &reader, (void*)i);
    }
    for (int i = 0; i < THREAD_NUM; i++) pthread_join(th[i], NULL);
    sem_destroy(&serviceQueue);
    sem_destroy(&resource);
    sem_destroy(&mutex);
    return 0;
}