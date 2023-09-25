/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:51:21 by toshota           #+#    #+#             */
/*   Updated: 2023/09/25 13:34:18 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	get_cmd_count(int argc, char **argv)
{
	int	cmd_count;
	int	i;

	cmd_count = 0;
	if (is_specified_here_doc(argv))
		i = 3;
	else
		i = 2;
	while (i < argc - 1)
	{
		if (is_cmd(argv[i]))
			cmd_count++;
		i++;
	}
	return (cmd_count);
}

static void	get_infile_fd(char **argv, int *infile_fd)
{
	if (is_specified_here_doc(argv))
	{
		*infile_fd = open_file(HERE_DOC_FILE_PATH, INFILE_HERE_DOC);
		proc_here_doc(argv[2], *infile_fd);
	}
	else
		*infile_fd = open_file(get_infile(argv), INFILE);
}

static void	get_outfile_fd(int argc, char **argv, int *outfile_fd)
{
	if (is_specified_here_doc(argv))
		*outfile_fd = open_file(get_outfile(argc, argv), OUTFILE_HERE_DOC);
	else
		*outfile_fd = open_file(get_outfile(argc, argv), OUTFILE);
}

static void	malloc_multiple_pipe(int argc, char **argv, t_data *data)
{
	int	i;

	i = 0;
	data->pipe_fd = (int **)malloc(sizeof(int *) * (get_cmd_count(argc, argv)
				+ 1));
	check_malloc(data->pipe_fd);
	while (i < get_cmd_count(argc, argv))
	{
		data->pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		check_malloc(data->pipe_fd);
		i++;
	}
	data->pipe_fd[i] = NULL;
}

void	get_data(int argc, char **argv, char **envp, t_data *data)
{
	get_cmd_absolute_path(argc, argv, envp, data);
	get_infile_fd(argv, &data->infile_fd);
	get_outfile_fd(argc, argv, &data->outfile_fd);
	malloc_multiple_pipe(argc, argv, data);
}
