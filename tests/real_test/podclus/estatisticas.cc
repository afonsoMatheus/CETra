#include "estatisticas.hh"

/****************************
 *      CONSTRUCTORS        *
 ****************************/
Estatisticas::Estatisticas(){
	Clear();
}

Estatisticas::Estatisticas( double soma, double soma_quadrados, double media, double desvio_padrao ){
	Estatisticas();

	soma_ = soma;
	soma_quadrados_ = soma_quadrados;
	media_ = media;
	desvio_padrao_ = desvio_padrao;
}

/****************************
 *        OPERATORS         *
 ****************************/

/****************************
 *     GETTERS & SETTERS    *
 ****************************/
double Estatisticas::soma(){
	return soma_;
}

double Estatisticas::soma_quadrados(){
	return soma_quadrados_;
}

double Estatisticas::media(){
	return media_;
}

double Estatisticas::desvio_padrao(){
	return desvio_padrao_;
}

unsigned long int Estatisticas::N(){
	return n_;
}

/****************************
 *         OTHER            *
 ****************************/

/**
 * Adds a new value and updates states.
 *
 * @param v: new value to be added
 */
void Estatisticas::Add( double v ){
	++n_;
	soma_ += v;
	soma_quadrados_ += v*v;

	//if( isinf( (long double) soma_) || isnan( (long double) soma_) )
		//cout << "ERRO INF or NAN: value=" << v << "; sum=" << soma_ << ";" << "sum^2=" << soma_quadrados_ << endl;
    //cout << "Estat N = " << n_ << endl;
	Update();
}

/**
 * Clear the value of all variables.
 */
void Estatisticas::Clear()
{
	soma_ = 0.0;
	soma_quadrados_ = 0.0;
	media_ = 0.0;
	desvio_padrao_ = 0.0;
	n_ = 0;
}

/**
 * Removes a value from the information and updates stats.
 *
 * @param v: value to be removed
 */
void Estatisticas::Remove( double v )
{
	--n_;
	soma_ -= v;
	soma_quadrados_ -= v*v;
	
	Update();
}

/**
 * Updates mean and standard deviation based on size, sum of elements and 
 * sum of square of elements.
 */
void Estatisticas::Update()
{
	media_ = soma_ / n_;

	if( n_ > 1 )
	{
		//Knowledge Discovery from data streams - chapter 2 - page 14
		desvio_padrao_ = soma_quadrados_;
		desvio_padrao_ -= pow(soma_, 2) / n_;
		desvio_padrao_ /=  n_ - 1;
		desvio_padrao_ = sqrt( desvio_padrao_ ); 
	}
	else
		desvio_padrao_ = 0;
}

/**
 * Merges two Estatisticas objects by summing up the information of 
 * elements and recalculating mean and standard deviation.
 *
 * @param e: Estatisticas object to be merged
 */
void Estatisticas::Fusion( Estatisticas e )
{
	n_ += e.N();
	soma_ += e.soma();
	soma_quadrados_ += e.soma_quadrados();

	Update();
}


string Estatisticas::ToStringHeader()
{
	stringstream out( stringstream::in | stringstream::out );

	out << "n,mean,\"standard deviation\",sum,\"sum of squares\"";
	
	return out.str();
}

string Estatisticas::ToString(){
	stringstream out( stringstream::in | stringstream::out );

	/*out << "n=" << n_ << "; ";
	out << "media=" << media_ << "; ";
	out << "dp=" << desvio_padrao_ << "; ";
	out << "sum=" << soma_ << "; ";
	out << "sum^2=" << soma_quadrados_ << ";";*/

	out <<  n_ << "," << media_ << "," << desvio_padrao_ << "," << soma_ << "," << soma_quadrados_;

	return out.str();
}

void Estatisticas::ToMap(Clustering &C){

	C.inter[0].push_back(n_);
	C.inter[1].push_back(media_);
	C.inter[2].push_back(desvio_padrao_);
	C.inter[3].push_back(soma_);
	C.inter[4].push_back(soma_quadrados_);
}
