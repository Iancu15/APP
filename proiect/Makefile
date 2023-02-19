build: gauss-jordan-serial gauss-jordan-openmp gauss-jordan-pthreads

gauss-jordan-serial: gauss-jordan-serial.c
	gcc gauss-jordan-serial.c -o gauss-jordan-serial

gauss-jordan-openmp: gauss-jordan-openmp.c
	gcc -fopenmp gauss-jordan-openmp.c -o gauss-jordan-openmp

gauss-jordan-pthreads: gauss-jordan-pthreads.c
	gcc gauss-jordan-pthreads.c -o gauss-jordan-pthreads -lpthread -lm

clean:
	rm gauss-jordan-serial gauss-jordan-openmp gauss-jordan-pthreads