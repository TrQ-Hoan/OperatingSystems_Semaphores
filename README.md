# Code Semaphores - Bài tập lớn Hệ Điều Hành

---

:warning: **Code chỉ chạy được trên môi trường Linux**

Ví dụ, biên dịch và chạy trương trình `binary_sema.c` bằng terminal:
```sh
# biên dịch
gcc -pthread binary_sema.c -o binary_sema_elf
# chạy trương trình
./binary_sema_elf
```

---

## Binary Semaphores (Khoá)

Code trong file `binary_sema.c`

## Semaphores For Ordering (Điều chỉnh đa tiến trình)

Code trong file `sem_for_ordering.c`

---

## Bài toán sản xuất tiêu dùng (bộ đệm giới hạn)

Code trong file `producer_consumer.c`

## Khoá đọc ghi

Code trong file `read_write_lock.c`

## Triết gia ăn cơm

Code trong file `philosophers.c`

## Điều chỉnh tiến trình

Code trong file `throttle.c`

---

## Triến khai Semaphores

Code trong file `zemaphore.h` và `zemaphore.c`

---

## Mở rộng (vấn đề khoá đọc ghi)

Code trong file `rw_fair.c`
