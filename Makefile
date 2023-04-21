all: main_server sub_1 sub_2 sub_3 client
main_server: main_server.c
	gcc -w -o main_server main_server.c -pthread

sub_1: sub_server_1.c
	gcc -w -o sub_1 sub_server_1.c -pthread

sub_2: sub_server_2.c
	gcc -w -o sub_2 sub_server_2.c -pthread

sub_3: sub_server_3.c
	gcc -w -o sub_3 sub_server_3.c -pthread

client: client.c
	gcc -w  -o client client.c -pthread

clean:
	rm -rf sub_1 sub_2 sub_3 client main_server