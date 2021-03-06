/*
 *  CAB403 Assignment - Semester 2 2015
 *
 *  Created by Lachlan Cesca & Shaun Karran
 */

/* ---- Includes ---- */
#include <errno.h>          /* For perror() */
#include <netdb.h>          /* For hostent struct */
#include <netinet/in.h>     /* Contains structures for internet domain addresses */
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>         /* To handle SIGINT */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>     /* Needed for using sockets */
#include <sys/types.h>      /* Contains definitions of data types used in system calls */
#include <termios.h>        /* To stop terminal from echoing ^C */
#include <unistd.h>         /* For close() */

#include "hangman.h"
#include "leaderboard.h"

/* ---- Defines ---- */
#define DEFAULT_PORT        "12345"
#define PORT_LENGTH         6
#define NO_FLAGS            0
#define LISTEN_BACKLOG      10
#define BUF_SIZE            256
#define DISCONNECT_SIGNAL   "QUIT"
#define PLAY_HANGMAN        1
#define SHOW_LEADERBOARD    2
#define QUIT                3
#define USERNAME_LENGTH     10
#define PASSWORD_LENGTH     10
#define MAX_CLIENTS         10

/* ---- Menu Graphics ---- */
const char WELCOME_MESSAGE[] = "\n"
                              "=============================================\n"
                              "\n"
                              " Welcome to the Online Hangman Gaming System\n"
                              "\n"
                              "=============================================\n"
                              "\n";

const char LOGIN_PROMPT[] = "You are required to logon with your username and password:\n";

const char USERNAME_PROMPT[] = "\nPlease enter your username --> ";
const char PASSWORD_PROMPT[] = "\nPlease enter your password --> ";

const char AUTH_FAILED[] = "\nYou entered either an incorrect username or password - disconnecting...\n";

const char MAIN_MENU[] = "\n"
                        "\n"
                        "===== Main Manu =====\n"
                        "\n"
                        "<1> Play Hangman\n"
                        "<2> Show Leaderboard\n"
                        "<3> Quit\n";

const char MENU_PROMPT[] = "\nSelection option (1 - 3) --> ";

const char NO_LEADERBOARD[] = "\n==============================================================================\n"
                              "\nThere is no information currently stored in the Leader Board. Try again later.\n"
                              "\n==============================================================================\n";

/* ---- Type Definitions ---- */
typedef struct {
    int         sock_fd;
    bool        connected;
    char        username[USERNAME_LENGTH];
    char        password[PASSWORD_LENGTH];
} Client_Info;

/* typedef to remove need for struct keyword. */
typedef struct addrinfo addrinfo;

/* ---- Function Declarations ---- */
void* handle_client(void *client_Info);

void add_client_to_queue(int sock_fd);

int get_client_from_queue();

bool play_hangman(Client_Info *client);

void send_leaderboard(Leaderboard *leaderboard, Client_Info *client);

int get_username(Client_Info *client);

int get_password(Client_Info *client);

bool authenticate_login(char *username, char *password);

int get_menu_selection(Client_Info *client);

void write_to_socket(int sock_fd, const char *str);

int read_from_socket(int sock_fd, char *str);

int create_passive_socket(char *port, addrinfo *addr);

void disconnect_client(Client_Info *client);

void shutdown_server(int sig);
