#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

// constants
#define EOS	257
#define NUM	258

typedef struct Node{
	int val;
	struct Node *next;
	struct Node *prev;
} node;


// prototypes 
node *expr();
node *term();
node *factor();
void error( char * );
int get_token();
void match( int );
node *addnodes(node *n1, node *n2);
node *multbydig(node *n1, int d);
node *multnodes(node *n1, node *n2);


// global communication variables
int current_token;
node *current_attribute;
FILE *ifp;
FILE *ofp;


/* bounded memory calculator */
int main( int argc, char *argv[] )
{	
	node *value;
	node *n;


	if (argc < 2){
		fprintf(stderr,"Not enough arguments\n");
		exit(1);
	}
	if (!(ifp = fopen(argv[1],"r"))) {
		fprintf(stderr,"Cannot open file %s\n",argv[1]);
		exit(1);
	}
	if (!(ofp = fopen(argv[2],"w"))) {
		fprintf(stderr,"Cannot open file %s\n",argv[2]);
		exit(1);
	}


	while (!feof(ifp)){
		current_token = get_token();
		while ( current_token != EOS ) {

			value = expr();
			
			for(n = value->next; n->val!=-1; n = n->next)
			{
				fprintf( ofp, "%d", n->val );
			}
			fprintf( ofp, "\n");
		}
	}
	fclose(ifp);
	fclose(ofp);
}

/* calculator */

// handles addition
node *expr()
{
	node *value = term();
	while (1) {
		if ( current_token == '+' ) {
			match( '+' );
			value = addnodes(value, term());
		}
		else
			break;
	}
	return value;
}

// handles multiplication
node *term()
{
	node *value = factor();
	while (1) {
		if ( current_token == '*' ) {
			match( '*' );
			value = multnodes(value, factor());
		}
		else break;
	}
	return value;
}

// handles brackets and numbers
node *factor()
{
	node *value;

	if ( current_token == '(' ) {
		match( '(' );
		value = expr();
		match( ')' );
		return value;
	}
	else if ( current_token == NUM ) {
		value = current_attribute;
		match( NUM );
		return value;
	}
	else error( "Unexpected token in factor()" );
}

/* match expected token */
void match( int expected_token )
{
	if ( current_token == expected_token ) {
		current_token = get_token();
	}
	else {
		error("Unexpected token in match" );
	}
}


/* get next token */
int get_token()
{
	int c;		// current character from the stream
	node *value;
	node *current;
	node *newnode;

	while (1) {
		switch ( c = fgetc(ifp) ) {
		case '+': case '*': case '(': case ')':
#ifdef DEBUG
			fprintf( stderr, "[OP:%c]", c );
#endif
			return c;	// return operators and brackets as is
		case ' ': case '\t':
			continue;	// ignore spaces and tabs
		default:
			if ( isdigit(c) ) {
				//headnode
				newnode = (node*)malloc(sizeof(node));
				newnode->val = -1;
				newnode->prev = newnode;
				newnode->next = newnode;
				value = newnode;

				//add first value
				newnode = (node*)malloc(sizeof(node));
				newnode->val = c - '0';
				newnode->next = value;
				newnode->prev = value;
				value->next = newnode;
				value->prev = newnode;
				current = newnode;


				while ( isdigit( c = fgetc(ifp) )) {
					newnode = (node*)malloc(sizeof(node));
					newnode->val = c - '0';
					newnode->next = value;
					newnode->prev = current;
					current->next = newnode;
					value->prev = newnode;
					current = newnode;
				}
				ungetc( c, ifp );

				current_attribute = value;




				return NUM;
			}
			else if ( c == '\n' || feof(ifp) ) {
				return EOS;
			}
			else {
				fprintf( ofp, "{%c}", c );
				error( "Unknown token" );
			}
		}
	}
}

//add function
node *addnodes(node *n1, node *n2)
{
	node *result = (node*)malloc(sizeof(node));
	result->val = -1;
	result->next = result;
	result->prev = result;

	node *newnode;
	node *current1 = n1->prev;
	node *current2 = n2->prev;
	int digit;
	bool carry = false;

	while(current1->val != -1 && current2->val !=-1)
	{
		digit = current1->val + current2->val;

		if(carry)
			digit++;

		newnode = (node*)malloc(sizeof(node));
		if(digit > 9)
		{
			carry = true;
			newnode->val = digit - 10;
			
		}
		else
		{
			carry = false;
			newnode->val = digit;
		}
		//add the node to the loop
		newnode->next = result->next;
		newnode->prev = result;
		result->next->prev = newnode;
		result->next = newnode;
		//update pointers
		current1 = current1->prev;
		current2 = current2->prev;
	}
	
	//if the first number still has more to add
	while(current1->val != -1)
	{
                digit = current1->val;

                if(carry)
                        digit++;

                newnode = (node*)malloc(sizeof(node));
                if(digit > 9)
                {
                        carry = true;
                        newnode->val = digit - 10;

                }
                else
                {
                        carry = false;
                        newnode->val = digit;
                }
                //add the node to the loop
                newnode->next = result->next;
                newnode->prev = result;
                result->next->prev = newnode;
                result->next = newnode;

                //update pointer
                current1 = current1->prev;
        }

	//if second num still has some left
	while(current2->val != -1)
	{
                digit = current2->val;

                if(carry)
                        digit++;

                newnode = (node*)malloc(sizeof(node));
                if(digit > 9)
                {
                        carry = true;
                        newnode->val = digit - 10;

                }
                else
                {
                        carry = false;
                        newnode->val = digit;
                }
                //add the node to the loop
                newnode->next = result->next;
                newnode->prev = result;
                result->next->prev = newnode;
                result->next = newnode;

                //update pointers
                current2 = current2->prev;
        }

	//check for final carry
	if(carry)
	{
		newnode = (node*)malloc(sizeof(node));
		newnode->val = 1;
		newnode->next = result->next;
                newnode->prev = result;
                result->next->prev = newnode;
                result->next = newnode;

	}

	return result;
}

//multiply helper fxn, handles num mult by one digit
node *multbydig(node *n1, int d)
{
	node *result = (node*)malloc(sizeof(node));
	result->val = -1;
	result->next = result;
	result->prev = result;
	node *newnode;

	int carry = 0;
	int value;
	node *n;
	for(n = n1->prev; n->val != -1; n = n->prev)
	{
		value = n->val * d + carry;
		carry = value / 10;
		newnode = (node*)malloc(sizeof(node));
		newnode->val = value % 10;
                newnode->next = result->next;
                newnode->prev = result;
                result->next->prev = newnode;
                result->next = newnode;

		
	}
	
	if(carry != 0)
	{
		newnode = (node*)malloc(sizeof(node));
                newnode->val = carry;
                newnode->next = result->next;
                newnode->prev = result;
                result->next->prev = newnode;
                result->next = newnode;

	}

	return result;

}

//multiply function
node *multnodes(node *n1, node *n2)
{
	node *result = (node*)malloc(sizeof(node));
	result->val = -1;
	result->next = result;
	result->prev = result;

	node *newnode;
	
	node *n;
	for(n = n2->prev; n->val != -1; n = n->prev)
	{
		result = addnodes(result, multbydig(n1,n->val));

		//add zero to end of n1 so next digit multiplies correctly
		newnode = (node*)malloc(sizeof(node));
                newnode->val = 0;
                newnode->next = n1;
                newnode->prev = n1->prev;
                n1->prev->next = newnode;
                n1->prev = newnode;

	}

	return result;
}


// error reporting function
void error( char *message )
{
	fprintf( ofp, "Error: %s\n", message );
	exit(1);
}

