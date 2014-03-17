/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Bjoern Geschka bjoern@dd-wrt.com, 2014
 */

#include <curl/curl.h>
#include <sys/stat.h>
#include "wgetssl.h"

#define DEBUG 0


/*******************************************************************
*            check if file is present, if not, create it
*******************************************************************/
int filePresenceCheck(char *filename)
{
    FILE *fp;
    struct stat statbuf;
    stat(filename, &statbuf);

    if(S_ISREG(statbuf.st_mode) == 0) {
        DEBUG == 1 ? fprintf(stderr, "%s\n", "file not present - created") : 0;
        fp = fopen(filename, "w");
        fclose(fp);
    }

    return S_ISREG(statbuf.st_mode);
}

/*******************************************************************
*               get file from url (https/SSL)
*******************************************************************/
int getFileFromSSLUrl(char *url, char *filename)
{
    FILE *fp;
    filePresenceCheck(filename);
    fp = fopen(filename, "w");
    CURL *curlpntr;
    curlpntr = curl_easy_init();

    if(curlpntr == NULL) {
        DEBUG == 1 ? fprintf(stderr, "%s\n","removing file") : 0;
        remove(filename);
        return 1;
    }

    curl_easy_setopt(curlpntr, CURLOPT_URL, url);
    curl_easy_setopt(curlpntr, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curlpntr, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curlpntr, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curlpntr, CURLOPT_WRITEDATA, fp);

    if(curl_easy_perform(curlpntr) != 0) {
        DEBUG == 1 ? fprintf(stderr, "%s\n", "curl failed") : 0;
        remove(filename);
        DEBUG == 1 ? fprintf(stderr, "%s\n","removing file") : 0;
        return 1;
    }

    curl_easy_cleanup(curlpntr);
    curl_global_cleanup();
    fclose(fp);
    return 0;
}
