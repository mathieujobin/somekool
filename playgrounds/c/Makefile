all:
	gcc -g hash_table.c && valgrind --leak-check=full ./a.out || echo "==============="

check_collisions:
	./a.out | grep index | sort -u | awk {'print $6'} | sort | uniq -c  | sort
