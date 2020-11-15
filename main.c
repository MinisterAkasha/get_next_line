#include "get_next_line.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int	main()
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line;
	int		ret;

	ret = 1;
	fd1 = open("TEST.TXT", O_RDONLY);
	fd2 = open("3TEST.TXT", O_RDONLY);
	fd3 = open("2TEST.TXT", O_RDONLY);
	while (ret > 0)
	{
		ret = get_next_line(fd1, &line);
		printf("%s\n", line);
		free(line);
	}
	// ret = 1;
	// while (ret > 0)
	// {
	// 	ret = get_next_line(fd2, &line);
	// 	printf("%s\n", line);
	// 	free(line);
	// }
	// ret = 1;
	// while (ret > 0)
	// {
	// 	ret = get_next_line(fd3, &line);
	// 	printf("%s\n", line);
	// 	free(line);
	// }
	// get_next_line(fd1, &line);
	// printf("%s\n", line);
	// free(line);
	// get_next_line(fd2, &line);
	// printf("%s\n", line);
	// free(line);
	// get_next_line(fd3, &line);
	// printf("%s\n", line);
	// free(line);
	close(fd1);
	close(fd2);
	close(fd3);
	// while (1)
	// {
		
	// }
	return (0);
}
// int	main()
// {
// 	int		fd1;
// 	char	*line;
// 	int		ret;

// 	ret = 1;
// 	fd1 = open("2TEST.TXT", O_RDONLY);
// 	while (ret > 0)
// 	{
// 		ret = get_next_line(fd1, &line);
// 		printf("%s\n", line);
// 		free(line);
// 	}
// 	get_next_line(fd1, &line);
// 	printf("%s\n", line);
// 	free(line);
// 	close(fd1);
// 	// while (1)
// 	// {
		
// 	// }
// 	return (0);
// }