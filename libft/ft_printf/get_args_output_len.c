/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_output_len.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 10:56:35 by tobeshota         #+#    #+#             */
/*   Updated: 2023/09/24 14:59:45 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	get_args_output_len(t_flags *flags)
{
	if (flags->precision == -2)
		flags->args = 0;
	if (flags->args > flags->precision && flags->precision > 0)
		flags->args = flags->precision;
}
