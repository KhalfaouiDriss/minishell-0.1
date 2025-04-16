/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexadecimal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/12/11 09:19:29 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/12/11 09:19:31 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthexadecimal(unsigned long long nb, int c)
{
	int		count;
	char	*base;

	count = 0;
	if (c == 'x' || c == 'p')
		base = "0123456789abcdef";
	else if (c == 'X')
		base = "0123456789ABCDEF";
	if (nb >= 16)
	{
		count += ft_puthexadecimal(nb / 16, c);
		count += ft_puthexadecimal(nb % 16, c);
	}
	else
	{
		count += ft_putchar(base[nb % 16]);
	}
	return (count);
}
