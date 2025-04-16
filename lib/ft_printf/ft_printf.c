/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalfao <dkhalfao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 09:19:13 by dkhalfao          #+#    #+#             */
/*   Updated: 2024/12/11 09:19:15 by dkhalfao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	check_format(char c, va_list list)
{
	unsigned long long	arg;

	if (c == 'c')
		return (ft_putchar(va_arg(list, int)));
	if (c == 's')
		return (ft_putstr(va_arg(list, char *)));
	if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(list, int)));
	if (c == 'u')
		return (ft_u_putnbr(va_arg(list, unsigned int)));
	if (c == 'p')
	{
		arg = va_arg(list, unsigned long long);
		if (arg == 0)
			return (write(1, "(nil)", 5));
		ft_putstr("0x");
		return (ft_puthexadecimal(arg, 'p') + 2);
	}
	if (c == 'x')
		return (ft_puthexadecimal(va_arg(list, unsigned int), c));
	if (c == 'X')
		return (ft_puthexadecimal(va_arg(list, unsigned int), c));
	if (c == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	list;
	int		i;
	int		size;

	i = 0;
	size = 0;
	va_start(list, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			if (format[i] == '%')
				size += ft_putchar('%');
			else
				size += check_format(format[i], list);
		}
		else
		{
			size += ft_putchar(format[i]);
		}
		i++;
	}
	va_end(list);
	return (size);
}
