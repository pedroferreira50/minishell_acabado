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