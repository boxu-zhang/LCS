#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int dump_index = 0;

void dump_table_change( int ** table, int m, int n )
{
    FILE * fp = fopen( "dump_table.txt", "a+" );

    if ( fp )
    {
        fseek( fp, 0, SEEK_END );

        fprintf( fp, "dump index %d\n", dump_index++ );

        for ( int i = 0; i < m + 1; ++i )
        {
            for ( int j = 0; j < n + 1; j++ )
            {
                fprintf( fp, "%2x ", table[ i ][ j ] );
            }

            fprintf( fp, "\n" );
        }
    }

    fclose( fp );
}

int max( int a, int b )
{
    return a > b ? a : b;
}

int ** lcs_table( char * x, char * y, int m, int n )
{
    if ( !x || !y )
        return NULL;

    int ** l_table = ( int ** )malloc( ( m + 1) * sizeof(int) );

    for ( int i = 0; i < n + 1; i++ )
    {
        l_table[ i ] = ( int * )malloc( (n + 1) * sizeof( int ) );
        
        memset( l_table[ i ], -1, ( n + 1 ) * sizeof( int ) );
    }

    for ( int i = 0; i < m + 1; i++ )
    {
        for ( int j = 0; j < n + 1; j++ )
        {
            if ( i == 0 || j == 0 )
                l_table[ i ][ j ] = 0;
            
            else if ( x[ i - 1 ] == y[ j - 1 ] )
                l_table[ i ][ j ] = l_table[ i - 1 ][ j - 1 ] + 1;
            
            else
                l_table[ i ][ j ] = max( l_table[ i - 1 ][ j ], l_table[ i ][ j - 1 ] );

            dump_table_change( l_table, m, n );
        }
    }

    return l_table;
}

int lcs( int ** lcs_table, int m, int n )
{
    if ( !lcs_table )
        return -1;

    return lcs_table[ m ][ n ];
}

char * lcs_str( char * x, char * y, int ** lcs_table, int m, int n )
{
    if ( !x || !y )
        return NULL;

    int lcs = lcs_table[ m ][ n ];
    char * lcs_str = ( char * )malloc( max( m, n ) * sizeof( char ) + 1 );
    int index = max( m, n );
    lcs_str[ index-- ] = '\0';

    for ( int i = m, j = n; i > 0 && j > 0; )
    {
        if ( x[ i - 1 ] == y[ j - 1 ] )
        {
            lcs_str[ index-- ] = x[ i - 1];
            i--;
            j--;
        }
            
        // follow the directions of the greater value
        if ( lcs_table[ i - 1 ][ j ] > lcs_table[ i ][ j - 1 ] )
            i--;
        else
            j--;
    }

    for ( int i = 0; i < max( m, n ); ++i )
    {
        if ( lcs_str[ index + 1 ] != '\0 ')
        {
            lcs_str[ i ] = lcs_str[ index + 1 ];
            ++index;
        }
    }
        
    return lcs_str;
}

int main( int argc, char * argv[] )
{
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";

    int m = strlen( X );
    int n = strlen( Y );

    int ** l_table = lcs_table( X, Y, m, n );
    char * l_str = lcs_str( X, Y, l_table, m, n );

    printf( "Length of LCS is %d\n", l_table[ m ][ n ] );
    printf( "String of LCS is %s\n", l_str );

    // clean up
    free( l_str );

    for ( int i = 0; i < m + 1; ++i )
        free( l_table[ i ] );

    free( l_table );

    return 0;
}