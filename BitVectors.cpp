#include<iostream>
#include<stdio.h>
#include <math.h>
#include <vector>
#include <array>

using namespace std;

/*

 

  */

class Bitvector
{
    public:
    unsigned long int* m_array;
    int m_size;

    void Resize(int p_size)
    {
        unsigned long int* newvector = 0;
        if (p_size % 32 == 0)
            p_size = p_size/32;
    
        else    
            p_size = (p_size/32) + 1;

        newvector = new unsigned long int[p_size];

        if(newvector == 0)
            return;
    
        int min;

        if(p_size<m_size)
            min = p_size;

        else   
            min =  m_size;

        int index;
        for(index = 0;index<min;index++)
        {
            newvector[index] = m_array[index];
        }

        m_size = p_size;

        if(m_array!=0)
        {
            delete[]m_array;
        }
        m_array = newvector;
    }

    Bitvector(int p_size)
    {
        m_array = 0;
        m_size = 0;
        Resize(p_size);
    }
    ~Bitvector()
    {
        if(m_array!=0)
            delete[] m_array;

        m_array = 0;
    }

    /*ACCESSING THE BIT...............pge 89
        There are several parts to retrieving an individual bit within a bitvector:
        1. Find the cell that the bit is in.
        2. Find which bit in the cell is the required one.
        3. Retrieve the bit.
        4. Shift it down so it has a value of 0 or 1.

        FOR 1: Divide the index by 32. 
            If index is 0-31 then it is the first cell.
            if index is 32-63 then 2nd cell and so on. 
        
        FOR 2:Modulo index by 32*/

    bool operator[] (int p_index)
    {
        int cell = p_index/32;
        int bit = p_index%32;
        return (m_array[cell] & (1 << bit)) >> bit; //binary & is applied
        /*For line 118: 
        First of all, you access the integer at index cell. This returns an integer.
        Next, you take 1 and shift it up bit spaces. Now, 
        this should give you a 1 at the same bit position as the bit you
        want to retrieve, right? Take a look at Figure 4.3 to see how this works.
        
        Then a binary & is applied to cell and the shifted 1 number. After this,
        the result obtained is the binary shifted number itself...

        to get the desired result, we shift down the result of 'binary &' and
        voila we get our answer. 
        */
    }

    /*SETTING THE BIT............pg 90,91,92
    Slightly complicated
    use '&' operator to clear bits and use the 
    'or' operator to set bits

               
    */

    void Set(int p_index, bool p_value)
    {
        int cell = p_index / 32;
        int bit = p_index % 32;
        
        if(p_value == true)
            m_array[cell] = (m_array[cell] | (1<<bit));
            /*
            you shift a 1 into the bit position that you want
            to set, and you logically or that with the correct cell. ....pg92
            */
        else
            m_array[cell] = (m_array[cell] & (~(1 << bit)));
            /*
            to clear the correct bit and keep all the other bits the same, ......pg92
            the bit you want to clear needs to be 0, and every other
            bit needs to be 1. Remember, using the logical and operator
            with a 1 is the identity function. Figure 4.5 demonstrates this algorithm.
            */
    }

    void Clearall()
    {
        for(int index = 0;index<m_size;index++)
        {
            m_array[index] = 0;
        }
    }

    void Setall()
    {
        for(int index = 0;index<m_size;index++)
        {
            m_array[index] = 0xFFFFFFFF; 
            // the hex for 1 in 32bit systems is 0x8timesF 
        }
    }

    //c code
    bool WriteFile( const char* p_filename )
    {
       FILE* outfile = 0;
       int written = 0;
        outfile = fopen( p_filename, "wb");
        if( outfile == 0 )
            return false;
        written = fwrite( m_array, sizeof(unsigned long int), m_size, outfile );
        fclose( outfile );
        if( written != m_size )
        return false;
        return true;
    }

    bool ReadFile( const char* p_filename )
    {
        FILE* infile = 0;
        int read = 0;
        infile = fopen( p_filename, "rb" );
        if( infile == 0 )
        return false;
        read = fread( m_array, sizeof(unsigned long int), m_size, infile );
        fclose( infile );
        if( read != m_size )
        return false;
        return true;
    }
    
};

class Player
{

    public:
    int exp;
    int life;
    int level;
    float money;

    float x_pos;
    float y_pos;

    Player()
    {
        exp = 0;
        life = 100;
        level = 0;
        money = 50.0;
        x_pos = 0;
        y_pos = 0;
    }
    
};


//Implementing quicksave
array<Player, 64> arrayPlayer;
Bitvector modifiedState(64);

void GameInit()
{
    //Setting random values
    for(int i = 0;i<64;i++)
    {
        //exp already initialised to 0
        //life already initialised to 100

        arrayPlayer[i].money = rand() % 100;
        arrayPlayer[i].level = 1 + (rand() % 5);


    }
}

void setMoney( int p_player, int p_money )
{
    arrayPlayer[p_player].money = p_money;
    modifiedState.Set( p_player, true );
}
void setExperience( int p_player, int p_exp )
{
    arrayPlayer[p_player].exp = p_exp;
    modifiedState.Set( p_player, true );
}
void setpos( int p_player, int p_x, int p_y )
{
    arrayPlayer[p_player].x_pos = p_x;
    arrayPlayer[p_player].y_pos = p_y;

    modifiedState.Set( p_player, true );
}

//Saving arrayPlayer to the disk
bool SavePlayers( const char* p_filename )
{
    int i;
    FILE* savefile = fopen( p_filename, "wb" );
    if( savefile == 0 )
        return false;

    for( i = 0; i < 64; i++ )
    {
        if( modifiedState[i] == true )
        {
            fseek( savefile, sizeof(Player) * i, SEEK_SET );//seek space in disk
            fwrite( &(arrayPlayer[i]), sizeof(Player), 1, savefile);//write player content into the disk
        }
    }

    modifiedState.Clearall();
    /* now all the players are upto date. If SavePlayer function
    is called twice consecutively then the second call wd do nothing*/

    return true;
}

int main()
{
    Bitvector bv(32);
    bool b;

    bv.Set(0, true);
    cout<<bv[0]<<endl;


    bv.Set(31, false);
    cout<<bv[31]<<endl;
    

}
