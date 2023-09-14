/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_number_conversion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 21:14:00 by tobeshota         #+#    #+#             */
/*   Updated: 2023/06/11 12:45:38 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	is_number_conversion(char c)
{
	return (c == 'd' || c == 'i' || c == 'u' || c == 'x' || c == 'X');
}
