/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 09:29:26 by toshota           #+#    #+#             */
/*   Updated: 2023/09/20 19:04:27 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define TOO_FEW_ARGC_ERROR \
	"argc at least 5 as follows: \
	./pipex infile cmd1 cmd2 outfile\n"
# define TOO_FEW_ARGC_ERROR_IN_BONUS \
	"argc at least 6 as follows: \
	./pipex here_doc LIMITTER cmd1 cmd2 outfile\n"
# define PERMISSION_DENIED_ERROR "permission denied\n"
# define FILE_OPEN_ERROR "unable to open file\n"
# define PATH_EXIST_ERROR "PATH not found\n"
# define FILE_EXIST_ERROR "file not found\n"
# define MALLOC_ERROR "failed to malloc\n"
# define CMD_ERROR "command not found\n"
# define TRUE 1
# define FALSE 0
# define INFILE	0
# define OUTFILE	1
# define OUTFILE_HERE_DOC	2

# define INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC 0

#endif
