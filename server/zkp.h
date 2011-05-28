#define TRUE 1
#define FALSE 0

/* Multiplicity of challenge */
#define K 1
/* Number of rounds of the protocol */
#define T 2
/* The port clients will connect to */
#define PORT "3490"  
/* How many pending connections the queue will hold */
#define BACKLOG 10
/* Max size in bytes we can receive */
#define MAXDATASIZE 1024 
/* Hard set modulus N */
#define N "179769313486231550853687999881939866971964108439943790719648214395408267051075942801914687894242121375217745092437482176397102348614986240522831137586986030596468029382843734495975025406176164494116255984153023325640732667334442837980102985095527876760493990494716878880715424438484827426446776351857492310737"

// int K;
mpz_t n;            /* modulus (a Blum integer) */
mpz_t i[K];         /* Prover's public key */
mpz_t rndseed;

void setrndseed();
void publish_modulus();
void compute_keys();
void timestamp(FILE *out);
char* itoa(int value, char* result, int base);

