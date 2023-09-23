/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 09:29:26 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 02:09:53 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>

typedef struct s_data
{
	pid_t	child_pid;
	int		infile_fd;
	int		outfile_fd;
	char	**cmd_absolute_path;
	char	**cmd_absolute_path_with_option;
	int		cmd_i;
	int		**pipe_fd;
}			t_data;

# define TRUE 1
# define FALSE 0

# define HERE_DOC_FILE_NAME "/tmp/here_doc"

# define INFILE 0
# define INFILE_HERE_DOC 1
# define OUTFILE 2
# define OUTFILE_HERE_DOC 3

# define INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC 0

#endif
