/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 06:57:40 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/04 06:57:40 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	handle_escape_sequences(const char *arg, t_indices *indices)
{
	if (arg[indices->i + 1] == '$')
	{
		indices->i += 2;
		return (1);
	}
	else if (arg[indices->i + 1] == '\\')
	{
		indices->i += 2;
		return (1);
	}
	else
	{
		indices->i += 2;
		return (2);
	}
}

static size_t	handle_quoted_content(const char *arg, t_indices *indices)
{
	size_t	start;
	size_t	end;

	start = indices->i + 2;
	end = start;
	while (arg[end] && arg[end] != '"')
		end++;
	if (arg[end] == '"')
	{
		indices->i = end + 1;
		return (end - start);
	}
	return (0);
}

static size_t	handle_dollar_expansion(const char *arg, t_indices *indices,
	t_shell *shell)
{
	size_t	quoted_size;

	if (arg[indices->i + 1] == '"')
	{
		quoted_size = handle_quoted_content(arg, indices);
		if (quoted_size > 0)
			return (quoted_size);
		else
			return (calc_var_size(arg, indices, shell));
	}
	else
	{
		return (calc_var_size(arg, indices, shell));
	}
}

static size_t	process_character_sequence(const char *arg, t_indices *indices,
	t_shell *shell)
{
	size_t	size;

	size = 0;
	while (arg[indices->i])
	{
		if (arg[indices->i] == '\\' && arg[indices->i + 1])
		{
			size += handle_escape_sequences(arg, indices);
			continue ;
		}
		if (arg[indices->i] == '$')
		{
			size += handle_dollar_expansion(arg, indices, shell);
			continue ;
		}
		size++;
		indices->i++ ;
	}
	return (size);
}

size_t	calc_expanded_size(const char *arg, char quote_type, t_shell *shell)
{
	size_t		size;
	t_indices	indices;

	if (!arg || !shell)
		return (1);
	if (quote_type == '\'')
		return (ft_strlen(arg) + 1);
	indices.i = 0;
	indices.j = 0;
	size = process_character_sequence(arg, &indices, shell);
	return (size + 1);
}
