/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 09:29:26 by toshota           #+#    #+#             */
/*   Updated: 2023/09/17 10:50:50 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define TOO_FEW_ARGC_ERROR \
	"argc at least 5 as follows: \
	./pipex file1 cmd1 cmd2 file2\n"
# define PERMISSION_DENIED_ERROR "permission denied\n"
# define FILE_OPEN_ERROR "unable to open file\n"
# define PATH_ERROR "PATH not found\n"

#define MALLOC_ERROR "failed to malloc\n"

#endif
