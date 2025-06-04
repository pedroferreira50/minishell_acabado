/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:19:34 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:19:35 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(char **args, t_command_data *data, t_indices *indices,
	t_shell *shell)
{
	if (validate_heredoc_args(args, indices, shell) == -1)
		return ;
	if (data->heredoc_delim)
	{
		free(data->heredoc_delim);
		data->heredoc_delim = NULL;
	}
	set_heredoc_delimiter(data, args[indices->i + 1]);
	if (data->heredoc_delim == NULL)
		shell->exit_status = 1;
	indices->i += 2;
}
