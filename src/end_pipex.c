/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_pipex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:54:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/25 14:32:51 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	all_free_int(int **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

void	end_pipex(char **argv, t_data *data)
{
	all_free_tab(data->cmd_absolute_path);
	all_free_tab(data->cmd_absolute_path_with_option);
	all_free_int(data->pipe_fd);
	if (is_specified_here_doc(argv))
		unlink(HERE_DOC_FILE_PATH);
	else
		check_close(close(data->infile_fd));
	check_close(close(data->outfile_fd));
}
