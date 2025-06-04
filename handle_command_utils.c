/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 03:10:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/04 03:10:00 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_command_data(t_command_data *data)
{
	if (data)
	{
		free_command_data(data);
		free(data);
	}
}

char	**parse_and_expand_command(char *input, t_shell *shell,
		t_parse_result *parsed)
{
	char	**expanded_args;
	int		count;

	*parsed = parse_command(input, shell);
	if (!parsed->args)
		return (NULL);
	expanded_args = expand_tokens(parsed->args, parsed->quote_types, shell);
	if (!expanded_args)
	{
		cleanup_parse_data(parsed, NULL, NULL);
		return (NULL);
	}
	count = 0;
	while (expanded_args[count])
		count++;
	if (count == 0)
	{
		cleanup_parse_data(parsed, expanded_args, NULL);
		return (NULL);
	}
	return (expanded_args);
}

char	**create_filtered_args(char **expanded_args)
{
	char	**filtered_args;
	int		i;
	int		j;
	int		count;

	count = 0;
	while (expanded_args[count])
		count++;
	filtered_args = malloc(sizeof(char *) * (count + 1));
	if (!filtered_args)
		return (NULL);
	i = 0;
	j = 0;
	while (expanded_args[i])
	{
		filtered_args[j] = ft_strdup(expanded_args[i]);
		j++;
		i++;
	}
	filtered_args[j] = NULL;
	return (filtered_args);
}

t_command_data	*prepare_command_data(char **filtered_args, t_shell *shell)
{
	t_command_data	*data;
	int				count;

	count = 0;
	while (filtered_args[count])
		count++;
	data = malloc(sizeof(t_command_data));
	if (!data)
		return (NULL);
	ft_memset(data, 0, sizeof(t_command_data));
	parse_input(filtered_args, count, data, shell);
	return (data);
}

void	execute_command_pipeline(t_command_data *data, t_shell *shell)
{
	/*if (shell->exit_status == 2)
	{
		cleanup_command_data(data);
		return ;
	}*/
	if (shell->exit_status != 2 && validate_command(data->commands, shell) == 0)
	{
		cleanup_command_data(data);
		return ;
	}
	execute_commands(data, shell);
	cleanup_command_data(data);
}
