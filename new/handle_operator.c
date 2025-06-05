#include "minishell.h"

void	handle_pipe(t_command_data *data, int *command_index, t_shell *shell)
{
	data->num_pipes++;
	(*command_index)++;
	shell->exit_status = 0;
}
