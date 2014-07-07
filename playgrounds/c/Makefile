all:
	gcc -g hash_table.c && valgrind --leak-check=full ./a.out || echo "==============="

ack:
	gcc -g ackerman.c && valgrind --leak-check=full ./a.out || echo "==============="

check_collisions:
	./a.out | grep index | sort -u | awk {'print $$7'} | sort | uniq -c  | sort
