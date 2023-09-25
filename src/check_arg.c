/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/25 14:32:31 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	is_infile_valid(char *infile)
{
	int	fd;

	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		fd = open_file(HERE_DOC_FILE_PATH, INFILE_HERE_DOC);
	else
		fd = open_file(infile, INFILE);
	check_close(close(fd));
	if (fd == -1)
		return (FALSE);
	return (TRUE);
}

static int	is_outfile_valid(char *infile, char *outfile)
{
	int	fd;

	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		fd = open_file(outfile, OUTFILE_HERE_DOC);
	else
		fd = open_file(outfile, OUTFILE);
	check_close(close(fd));
	if (fd == -1)
		return (FALSE);
	return (TRUE);
}

static int	is_argc_valid(int argc, char **argv)
{
	if (argc > 1 && is_specified_here_doc(argv) && argc < 6)
	{
		put_error("argc at least 6 as follows:\n\
./pipex here_doc LIMITTER cmd1 cmd2 outfile\n");
		return (FALSE);
	}
	if (argc < 5)
	{
		put_error("argc at least 5 as follows:\n\
./pipex infile cmd1 cmd2 outfile\n");
		return (FALSE);
	}
	return (TRUE);
}

static int	is_argv_valid(int argc, char **argv)
{
	if (is_outfile_valid(get_infile(argv), get_outfile(argc, argv)) == FALSE)
		return (FALSE);
	if (is_infile_valid(get_infile(argv)) == FALSE)
		return (FALSE);
	return (TRUE);
}

void	check_arg(int argc, char **argv)
{
	if (is_argc_valid(argc, argv) == FALSE)
		exit(1);
	if (is_argv_valid(argc, argv) == FALSE)
		exit(1);
}
