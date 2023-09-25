/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/25 14:33:01 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	is_specified_here_doc(char **argv)
{
	return (ft_strlen(argv[1]) == ft_strlen("here_doc") && (!ft_strncmp(argv[1],
				"here_doc", ft_strlen(argv[1]))));
}

void	proc_here_doc(char *limitter, int infile_fd)
{
	char	*line;
	char	*limitter_endl;

	line = get_next_line(STDIN_FILENO);
	limitter_endl = ft_strjoin(limitter, "\n");
	check_malloc(limitter_endl);
	while (ft_strncmp(line, limitter_endl, ft_strlen(line)))
	{
		ft_putstr_fd(line, infile_fd);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(limitter_endl);
	check_close(close(infile_fd));
	infile_fd = open_file(HERE_DOC_FILE_PATH, INFILE_HERE_DOC);
}
