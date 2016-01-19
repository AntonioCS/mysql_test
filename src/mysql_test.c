
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>

static char *opt_host_name = "127.0.0.1"; /* server host (default=localhost) */
static char *opt_user_name = "root"; /* username (default=login name) */
static char *opt_password = "123"; /* password (default=none) */
static unsigned int opt_port_num = 0; /* port number (use built-in value) */
static char *opt_socket_name = NULL; /* socket name (use built-in value) */
static char *opt_db_name = "classicmodels"; /* database name (default=none) */
static unsigned int opt_flags = 0; /* connection flags (none) */

static MYSQL *conn; /* pointer to connection handler */

int
main(int argc, char *argv[]) {
    MY_INIT(argv[0]);
    
    /* initialize client library */
    if (mysql_library_init(0, NULL, NULL)) {
        fprintf(stderr, "mysql_library_init() failed\n");
        exit(1);
    }
    /* initialize connection handler */
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
        exit(1);
    }

    if (conn == NULL) {
        printf("Unable to connect\n");
        exit(1);
    }

    /* connect to server */
    if (mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password,
            opt_db_name, opt_port_num, opt_socket_name, opt_flags) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(1);
    } else {
        printf("Connected\n");
        MYSQL_RES *res_set;

        char *sql = "SELECT * "
                "FROM offices"
                ;

        if (mysql_query(conn, sql) != 0) {
            printf("mysql_query() failed\n");
        } else {
            res_set = mysql_store_result(conn); /* generate result set */
            if (res_set == NULL)
                printf("mysql_store_result() failed\n");
            else {
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res_set)) != NULL) {
                    for (int i = 0; i < mysql_num_fields(res_set); i++) {
                        if (i > 0) {
                            fputc('\t', stdout);
                        }

                        printf("%s", row[i] != NULL ? row[i] : "NULL");
                    }
                    fputc('\n', stdout);
                }
                if (mysql_errno(conn) != 0)
                    printf("mysql_fetch_row() failed\n");
                else
                    printf("Number of rows returned: %lu\n",
                        (unsigned long) mysql_num_rows(res_set));
            }
            mysql_free_result(res_set);
        }
    }
    
    mysql_close(conn);
    mysql_library_end();
    exit(0);
}
