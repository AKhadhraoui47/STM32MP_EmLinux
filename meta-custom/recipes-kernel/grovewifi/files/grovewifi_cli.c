/*
- Multi steps commands: make it SoftAp
- pwd formatting for special caracters
- Commands frequency
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SYSFS_CLI_PATH "/sys/kernel/grovewifiv2/cmd_cli"
#define SYSFS_RSP_PATH "/sys/kernel/grovewifiv2/response_cli"
#define SYSFS_ID_PATH "/sys/kernel/grovewifiv2/cli_id"

#define GROVEWIFI_CLI_MAX_LENGTH 15
#define GROVEWIFI_RSP_MAX_LENGTH 35
#define GROVEWIFI_CMD_MAX_LENGTH 55

enum grovewifi_cli {
    CLI_CHK_STATE,
    CLI_CONNECT,
    CLI_DISCONNECT,
    CLI_PING,
    CLI_GET_IP,
    CLI_UNDEF,
};

static const char grovewifi_cli_tbl[][GROVEWIFI_CLI_MAX_LENGTH] = {
    [CLI_CHK_STATE] = "--chk-state",
    [CLI_CONNECT] = "--connect",
    [CLI_DISCONNECT] = "--disconnect",
    [CLI_PING] = "--ping",
    [CLI_GET_IP] = "--get-ip",
}; 

struct grovewifi_cli_state {
    enum grovewifi_cli command;
    char **variables;
    int var_nb;
};

static void grovewifi_write_cli_to_sysfs(const char *cli) {
    int fd = open(SYSFS_CLI_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open sysfs parameter file");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(fd, cli, strlen(cli));
    /*if (bytes_written == -1) {
        perror("CLI Failed to write to sysfs parameter file");
        close(fd);
        exit(EXIT_FAILURE);
    }*/

    close(fd);
}

static void grovewifi_write_id_to_sysfs(unsigned int id) {
    int fd = open(SYSFS_ID_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open sysfs parameter file");
        exit(EXIT_FAILURE);
    }
    char id_str[3];
    sprintf(id_str,"%d",id);
    ssize_t bytes_written = write(fd, id_str, strlen(id_str));
    if (bytes_written == -1) {
        perror("ID Failed to write to sysfs parameter file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

static void grovewifi_process_response_sysfs(enum grovewifi_cli cli) {
    int fd = open(SYSFS_RSP_PATH, O_RDONLY);
    char response[GROVEWIFI_RSP_MAX_LENGTH]; 
    if (fd == -1) {
        perror("Failed to open sysfs response file");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(fd, response, GROVEWIFI_RSP_MAX_LENGTH);
    if (bytes_read == -1) {
        perror("RSP Failed to read from sysfs response file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Response: %s\n", response);

    close(fd);
}

enum grovewifi_cli grovewifi_cli_to_enum(const char *cli_s) {
    for (enum grovewifi_cli cli = CLI_CHK_STATE; cli <= CLI_GET_IP; cli++) {
        if (!strcmp(grovewifi_cli_tbl[cli], cli_s)) {
            return cli;
        }
    }
    return CLI_UNDEF;
}

static void grovewifi_format_atcommand(struct grovewifi_cli_state cli_state, char **at_cmd) {
    enum grovewifi_cli cli_enum = cli_state.command;

    *at_cmd = (char *)malloc(GROVEWIFI_CMD_MAX_LENGTH * sizeof(char));
    if (*at_cmd == NULL) {
        fprintf(stderr, "Command failed: Memory alloc error\n");
        exit(EXIT_FAILURE);
    }

    switch (cli_enum) {
    case CLI_CHK_STATE:
        if (cli_state.var_nb != 0) {
            fprintf(stderr, "More than expected arguments: run: grovewifi --chk-state\n");
            free(*at_cmd); 
            exit(EXIT_FAILURE);
        }
        strcpy(*at_cmd, "AT");
        break;
    case CLI_CONNECT:
        if (cli_state.var_nb < 2) {
            fprintf(stderr, "Not enough parameters passed: run: grovewifi --connect <ssid> <password>\n");
            free(*at_cmd); 
            exit(EXIT_FAILURE);
        }
        snprintf(*at_cmd, GROVEWIFI_CMD_MAX_LENGTH, "AT+CWJAP=\"%s\",\"%s\"", cli_state.variables[0], cli_state.variables[1]);
        break;
    case CLI_PING:
        if (cli_state.var_nb != 1) {
            fprintf(stderr, "Unexpected arguments: run: grovewifi --ping <ip or host>\n");
            free(*at_cmd); 
            exit(EXIT_FAILURE);
        }
        snprintf(*at_cmd, GROVEWIFI_CMD_MAX_LENGTH, "AT+PING=\"%s\"", cli_state.variables[0]);
        break;
    case CLI_DISCONNECT:
        if (cli_state.var_nb > 0) {
            fprintf(stderr, "Unexpected arguments: run: grovewifi --disconnect\n");
            free(*at_cmd); 
            exit(EXIT_FAILURE);
        }
        snprintf(*at_cmd, GROVEWIFI_CMD_MAX_LENGTH, "AT+CWQAP");
        break;
    case CLI_GET_IP:
        if (cli_state.var_nb > 0) {
            fprintf(stderr, "Unexpected arguments: run: grovewifi --get-ip\n");
            free(*at_cmd); 
            exit(EXIT_FAILURE);
        }
        snprintf(*at_cmd, GROVEWIFI_CMD_MAX_LENGTH, "AT+CIPSTA?");
        break;    
    default:
        fprintf(stderr, "Unhandled command\n");
        free(*at_cmd); 
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "No command defined: run: grovewifi help\n");
        exit(EXIT_FAILURE);
    }

    const char *cli = argv[1];
    const enum grovewifi_cli cli_enum = grovewifi_cli_to_enum(cli);

    if (cli_enum == CLI_UNDEF) {
        fprintf(stderr, "Unknown command: run: grovewifi help\n");
        exit(EXIT_FAILURE);
    }

    struct grovewifi_cli_state cli_state;
    cli_state.command = cli_enum;
    cli_state.var_nb = argc - 2;
    cli_state.variables = &(argv[2]);

    char *at_cmd = NULL;

    grovewifi_format_atcommand(cli_state, &at_cmd);
    grovewifi_write_id_to_sysfs((unsigned int) cli_state.command);
    grovewifi_write_cli_to_sysfs(at_cmd);
    grovewifi_process_response_sysfs(cli_state.command);

    free(at_cmd);

    return 0;
}
