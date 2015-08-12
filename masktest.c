/*

  .....____    ...::$$$$=$=$=$  ...____
_____/    /__        $$     $$___/    /___
\           //\\____$$   __$$\__         /
 \__     __//        /  / $$__  /    ___/
   /    /___\   \.   ) / $$  / /    /___
  /        /    /   / / $$  __/        /
  \_______....  ___/ / $$_   \\_______/
          /    /$$=$=$=$/ \   \
      ...(____/           /   /
                 .......:::./

    The Application For Mask Testing
    is the part of mapimp plugin
    coded by takerZ
    tPORt, 2009-2012

YOU CAN USE THE SOURCE ANY WAY YOU LIKE WITHOUT
RESTRICTIONS, BUT STILL REMEMBER THAT THIS SOFTWARE
IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE BLAH BLAH BLAH
*/

#include <stdio.h>
#include <string.h>

#include "mapimp.c"

#ifndef MASKTEST
#include <windows.h>
#else

/* Some usage info */
void usage(TCHAR* arg)
{
    TCHAR* p = strrchr(arg, '\\'), *ext = strrchr(arg, '.');
    if (!p)
    {
        p = arg;
    }
    else
    {
        p++;
    }
    if (ext)
    {
        *ext = '\0';
    }
    printf("%s [options] names masks\n"
        "  -m (--from-map)    the names input file is a map file\n", p);
}

#endif

int main(int argc, TCHAR** argv)
{

#ifdef MASKTEST

    FILE* nm_file, *msk_file, *out_file;
    TCHAR buffer[MAPBUFLEN];
    TCHAR* nm_buf, *msk_buf;
    list_t* names = NULL;
    mask_t* msk;
    name_t* nm;
    int delta, opt = 0;

    /* Checking argument count */
    if (argc < 3 || argc > 4)
    {
        usage(argv[0]);
        return 0;
    }

    /* Detecting whether user tries to pass some options as the first argument */
    if (argv[1][0] == '-')
    {
        if (argv[1][1] == 'M' || argv[1][1] == 'm' || !strcmpi(argv[1], "--from-map"))
        {
            opt++;
            if (argc < 4)
            {
                usage(argv[0]);
                return 0;
            }
        }
        else
        {
            usage(argv[0]);
            return 0;
        }
    }

    /* Trying to open input files */
    if (!(msk_file = fopen(argv[opt + 2], "r")))
    {
        printf("Could not open %s. Aborting test.\n", argv[opt + 2]);
        return 0;
    }

    /* Reading names from input file and adding them to a list */
    if (opt)
    {
        names = mapfile_parse(argv[opt + 1]);
    }
    else
    {
        if (nm_file = fopen(argv[opt + 1], "r"))
        {
            names = list_create();
            while (fgets(buffer, MAPBUFLEN, nm_file))
            {
                if (!(nm_buf = strtok(buffer, "\n")))
                {
                    continue;
                }
                list_add_name(names, nm_buf, strlen(nm_buf), 0, 0);
            }
            fclose(nm_file);
        }
    }
    if (!names)
    {
        printf("Could not open %s. Aborting test.\n", argv[opt + 1]);
        return 0;
    }

    /* Reading masks from input file and adding them to a list */
    g_Config = malloc(sizeof(config_t));
    g_Config->masks = list_create();
    while (fgets(buffer, MAPBUFLEN, msk_file))
    {
        if (!(msk_buf = strtok(buffer, "\n")))
        {
            continue;
        }
        printf("%69s", msk_buf);
        msk = list_add_mask(g_Config->masks, msk_buf);

        /* If mask compilation failed, then printing some info about an error */
        if (msk->errcode)
        {
            mask_error(msk, buffer);
            printf(" -- FAILED\nERROR: %s\n\n", buffer);

            /*
                list_addmasks does not free allocated buffer, cuz we may want to read the
                info about an error. So when we read it, we have to free it by ourselves
            */
            free(msk);
        }
        else
        {
            printf(" -- OK\n");
        }

    }
    fclose(msk_file);

    /* Charging the lazor!!1 */
    printf("\nTesting..");
    delta = GetTickCount();

    for (nm = names->first; nm; nm = nm->next)
    {
        mask_filter(nm);
    }
    delta = GetTickCount() - delta;
    printf(". ended %d names in %d milliseconds.\n\n", names->count, delta);

    /* Creating output file. Let it be called the same way as names file, but with 2 undescores at the end */
    if (!(out_file = fopen(strcat(argv[opt + 1], "__"), "w+")))
    {
        printf("Could not create the output file, anybody cares?\n");
        return 0;
    }

    /*
        Writing filtered names to the output file. The reason why we haven't done this
        in the filtering loop is that we were trying to get clear time measurement
    */
    for (nm = names->first; nm; nm = nm->next)
    {
        fprintf(out_file, "%s\n", nm->buffer);
    }
    fclose(out_file);

    /* Destroying lists */
    list_free_names(names);
    list_free_masks(g_Config->masks);

    return delta;

#else

    printf("Mask testing disabled.\n\n");
    return 0;

#endif

}