/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 12:53:17 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*get_infile(char **argv)
{
	char	*infile;

	if (is_specified_here_doc(argv))
		infile = INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC;
	else
		infile = argv[1];
	return (infile);
}

char	*get_outfile(int argc, char **argv)
{
	return (argv[argc - 1]);
}
