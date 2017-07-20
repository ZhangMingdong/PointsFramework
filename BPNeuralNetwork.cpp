#include "BPNeuralNetwork.h"

#include <stdio.h>
#include<stdlib.h>
#include <string>
#include <iostream>

#include <math.h>



#define TARGET_HIGH 0.9
#define TARGET_LOW 0.1

using namespace std;


#define fastcopy(to,from,len)\
{\
  register char *_to,*_from;\
  register int _i,_l;\
  _to = (char *)(to);\
  _from = (char *)(from);\
  _l = (len);\
  for (_i = 0; _i < _l; _i++) *_to++ = *_from++;\
}

/*** Return random number between 0.0 and 1.0 ***/
double drnd()
{
	return ((double)rand() / RAND_MAX);
}

/*** Return random number between -1.0 and 1.0 ***/
double dpn1()
{
	return ((drnd() * 2.0) - 1.0);
}

/*** The squashing function.  Currently, it's a sigmoid. ***/
double squash(double x)
{
	return (1.0 / (1.0 + exp(-x)));
}

/*** Allocate 2d array of doubles ***/
double** Create2DArray(int m, int n)
{
	double** buf = new double*[m];
	for (int i = 0; i < m; i++) {
		buf[i] = new double[n];
	}

	return buf;
}

void bpnn_randomize_weights(double **w, int m, int n)
{
	for (int i = 0; i <= m; i++) {
		for (int j = 0; j <= n; j++) {
			w[i][j] = 0.1 * dpn1();
		}
	}
}

void bpnn_zero_weights(double **w, int m, int n)
{
	for (int i = 0; i <= m; i++) {
		for (int j = 0; j <= n; j++) {
			w[i][j] = 0.0;
		}
	}
}

void bpnn_layerforward(double *l1, double *l2, double **conn, int n1, int n2)
{
	/*** Set up thresholding unit ***/
	l1[0] = 1.0;

	/*** For each unit in second layer ***/
	for (int j = 1; j <= n2; j++) {

		/*** Compute weighted sum of its inputs ***/
		double sum = 0.0;
		for (int k = 0; k <= n1; k++) {
			sum += conn[k][j] * l1[k];
		}
		l2[j] = squash(sum);
	}

}

// o(1-o)(t-o)
void bpnn_output_error(double *delta, double *target, double *output, int nj, double *err)
{
	double errsum = 0.0;
	for (int j = 1; j <= nj; j++) {
		double o = output[j];
		double t = target[j];
		delta[j] = o * (1.0 - o) * (t - o);
		errsum += abs(delta[j]);
	}
	*err = errsum;
}

void bpnn_hidden_error(double *delta_h, int nh, double *delta_o, int no, double **who, double *hidden, double *err)
{
	double errsum = 0.0;
	for (int j = 1; j <= nh; j++) {
		double h = hidden[j];
		double sum = 0.0;
		for (int k = 1; k <= no; k++) {
			sum += delta_o[k] * who[j][k];
		}
		delta_h[j] = h * (1.0 - h) * sum;
		errsum += abs(delta_h[j]);
	}
	*err = errsum;
}

void bpnn_adjust_weights(double *delta, int ndelta, double *ly, int nly, double **w, double **oldw, double eta, double momentum)
{
	ly[0] = 1.0;
	for (int j = 1; j <= ndelta; j++) {
		for (int k = 0; k <= nly; k++) {
			double new_dw = ((eta * delta[j] * ly[k]) + (momentum * oldw[k][j]));
			w[k][j] += new_dw;
			oldw[k][j] = new_dw;
		}
	}
}



void BPNeuralNetwork::Initialize(unsigned int seed)
{
	printf("Random number generator seed: %d\n", seed);
	srand(seed);
}

BPNeuralNetwork::BPNeuralNetwork(int n_in, int n_hidden, int n_out)
{
	this->input_n = n_in;
	this->hidden_n = n_hidden;
	this->output_n = n_out;

	this->input_units = new double[n_in + 1];
	this->hidden_units = new double[n_hidden + 1];
	this->output_units = new double[n_out + 1];

	this->hidden_delta = new double[n_hidden + 1];
	this->output_delta = new double[n_out + 1];
	this->target = new double[n_out + 1];

	this->input_weights = Create2DArray(n_in + 1, n_hidden + 1);
	this->hidden_weights = Create2DArray(n_hidden + 1, n_out + 1);

	this->input_prev_weights = Create2DArray(n_in + 1, n_hidden + 1);
	this->hidden_prev_weights = Create2DArray(n_hidden + 1, n_out + 1);
}

void BPNeuralNetwork::FeedForward()
{
	/*** Feed forward input activations. ***/
	bpnn_layerforward(this->input_units, this->hidden_units, this->input_weights, input_n, hidden_n);
	bpnn_layerforward(this->hidden_units, this->output_units, this->hidden_weights, hidden_n, output_n);

}

void BPNeuralNetwork::UpdateBackward(double eta, double momentum, double *eo, double *eh)
{
	// 1.Compute error and delta on output and hidden units.
	bpnn_output_error(this->output_delta, this->target, this->output_units, output_n, eo);
	bpnn_hidden_error(this->hidden_delta, hidden_n, this->output_delta, output_n, this->hidden_weights, this->hidden_units, eh);

	// 2.Adjust input and hidden weights.
	bpnn_adjust_weights(this->output_delta, output_n, this->hidden_units, hidden_n, this->hidden_weights, this->hidden_prev_weights, eta, momentum);
	bpnn_adjust_weights(this->hidden_delta, hidden_n, this->input_units, input_n, this->input_weights, this->input_prev_weights, eta, momentum);

}

void BPNeuralNetwork::Save(char *filename)
{
	int n1, n2, n3, i, j, memcnt;
	double dvalue, **w;
	char *mem;
	FILE* fd;

	if ((fd = fopen(filename, "wb")) == NULL) {
		printf("BPNeuralNetwork_SAVE: Cannot create '%s'\n", filename);
		return;
	}

	n1 = this->input_n;  n2 = this->hidden_n;  n3 = this->output_n;
	printf("Saving %dx%dx%d network to '%s'\n", n1, n2, n3, filename);
	fflush(stdout);

	fwrite((char *)&n1, sizeof(int), 1, fd);
	fwrite((char *)&n2, sizeof(int), 1, fd);
	fwrite((char *)&n3, sizeof(int), 1, fd);

	memcnt = 0;
	w = this->input_weights;
	mem = (char *)malloc((unsigned)((n1 + 1) * (n2 + 1) * sizeof(double)));
	for (i = 0; i <= n1; i++) {
		for (j = 0; j <= n2; j++) {
			dvalue = w[i][j];
			fastcopy(&mem[memcnt], &dvalue, sizeof(double));
			memcnt += sizeof(double);
		}
	}
	fwrite(mem, (n1 + 1) * (n2 + 1) * sizeof(double), 1, fd);
	free(mem);

	memcnt = 0;
	w = this->hidden_weights;
	mem = (char *)malloc((unsigned)((n2 + 1) * (n3 + 1) * sizeof(double)));
	for (i = 0; i <= n2; i++) {
		for (j = 0; j <= n3; j++) {
			dvalue = w[i][j];
			fastcopy(&mem[memcnt], &dvalue, sizeof(double));
			memcnt += sizeof(double);
		}
	}
	fwrite(mem, (n2 + 1) * (n3 + 1) * sizeof(double), 1, fd);
	free(mem);

	fclose(fd);
	return;
}

BPNeuralNetwork* BPNeuralNetwork::Create(int n_in, int n_hidden, int n_out) {

	BPNeuralNetwork *newnet = new BPNeuralNetwork(n_in, n_hidden, n_out);

	//#define INITZERO

#ifdef INITZERO
	bpnn_zero_weights(newnet->input_weights, n_in, n_hidden);
#else
	bpnn_randomize_weights(newnet->input_weights, n_in, n_hidden);
#endif
	bpnn_randomize_weights(newnet->hidden_weights, n_hidden, n_out);
	bpnn_zero_weights(newnet->input_prev_weights, n_in, n_hidden);
	bpnn_zero_weights(newnet->hidden_prev_weights, n_hidden, n_out);

	return (newnet);
}

BPNeuralNetwork* BPNeuralNetwork::Read(char *filename) {
	int n1, n2, n3;
	FILE *fd = fopen(filename, "rb");

	if (fd == NULL) {
		return (NULL);
	}

	printf("Reading '%s'\n", filename); 

	fread((char *)&n1, sizeof(int), 1, fd);
	fread((char *)&n2, sizeof(int), 1, fd);
	fread((char *)&n3, sizeof(int), 1, fd);
	BPNeuralNetwork *newptr = new BPNeuralNetwork(n1, n2, n3);

	printf("'%s' contains a %dx%dx%d network\n", filename, n1, n2, n3);
	printf("Reading input weights...");  

	int memcnt = 0;
	char *mem = (char *)malloc((unsigned)((n1 + 1) * (n2 + 1) * sizeof(double)));
	fread(mem, (n1 + 1) * (n2 + 1) * sizeof(double), 1, fd);
	for (int i = 0; i <= n1; i++) {
		for (int j = 0; j <= n2; j++) {
			fastcopy(&(newptr->input_weights[i][j]), &mem[memcnt], sizeof(double));
			memcnt += sizeof(double);
		}
	}
	free(mem);

	printf("Done\nReading hidden weights...");  

	memcnt = 0;
	mem = (char *)malloc((unsigned)((n2 + 1) * (n3 + 1) * sizeof(double)));
	fread(mem, (n2 + 1) * (n3 + 1) * sizeof(double), 1, fd);
	for (int i = 0; i <= n2; i++) {
		for (int j = 0; j <= n3; j++) {
			fastcopy(&(newptr->hidden_weights[i][j]), &mem[memcnt], sizeof(double));
			memcnt += sizeof(double);
		}
	}
	free(mem);
	fclose(fd);

	printf("Done\n"); 

	bpnn_zero_weights(newptr->input_prev_weights, n1, n2);
	bpnn_zero_weights(newptr->hidden_prev_weights, n2, n3);

	return (newptr);
}

BPNeuralNetwork::~BPNeuralNetwork()
{
	int n1, n2, i;

	n1 = this->input_n;
	n2 = this->hidden_n;

	free((char *)this->input_units);
	free((char *)this->hidden_units);
	free((char *)this->output_units);

	free((char *)this->hidden_delta);
	free((char *)this->output_delta);
	free((char *)this->target);

	for (i = 0; i <= n1; i++) {
		free((char *)this->input_weights[i]);
		free((char *)this->input_prev_weights[i]);
	}
	free((char *)this->input_weights);
	free((char *)this->input_prev_weights);

	for (i = 0; i <= n2; i++) {
		free((char *)this->hidden_weights[i]);
		free((char *)this->hidden_prev_weights[i]);
	}
	free((char *)this->hidden_weights);
	free((char *)this->hidden_prev_weights);
}

int BPNeuralNetwork::evaluatePerformance(double *err)
{
	*err = 0;
	int nResult = 1;
	for (size_t i = 1; i <= output_n; i++)
	{
//		double dbTarget1 = target[i];
//		double dbTarget2 = output_units[i];
//		cout << this->target[i] <<"\t"<<this->output_units[i] << endl;

		double delta = this->target[i] - this->output_units[i];
		*err  +=(0.5 * delta * delta);
		nResult = nResult && (this->target[i] > 0.5 == this->output_units[i] > 0.5);
	}
//	cout << endl;

	return nResult;
}

int BPNeuralNetwork::CalculateLabel() {
	int nlabel = 1;
	double dbMax = output_units[1];
	for (size_t i = 2; i <= output_n; i++)
	{
		if (output_units[i] > dbMax) {
			nlabel = i;
			dbMax = output_units[i];
		}
	}
	return nlabel;
}


/*** Computes the performance of a net on the images in the imagelist. ***/
/*** Prints out the percentage correct on the image set, and the
average error between the target and the output units for the set. ***/
void BPNeuralNetwork::CalculatePerformance(const std::vector<LabeledVector*>& vecLV, int list_errors)
{
	double err = 0.0;			// total error
	int correct = 0;			// number of correct
	int nInputSize = vecLV.size();
	if (nInputSize <= 0) {
		if (!list_errors)
			printf("0.0 0.0 ");
		return;
	}

	// calculate every input image
	for (int i = 0; i < nInputSize; i++) {
		// 0.Load the input and output layer.
		LoadInputData(vecLV[i]);

		// 1.Run the network on this input.
		FeedForward();

		double val;
		if (this->evaluatePerformance(&val)) {
			correct++;
		}
		err += val;
	}

	// calculate average
	err = err / (double)nInputSize;

	if (!list_errors)
		/* bthom==================================
		this line prints part of the ouput line
		discussed in section 3.1.2 of homework
		*/
		printf("%g %g ", ((double)correct / (double)nInputSize) * 100.0, err);

}

void BPNeuralNetwork::LoadInputData(const LabeledVector* pLV) {

	for (size_t i = 0; i < input_n; i++)
	{
		input_units[i+1] = pLV->GetData(i);
	}


	for (size_t i = 0; i < pLV->GetK(); i++)
	{
		this->target[i + 1] = TARGET_LOW;
	}
	this->target[pLV->GetLabel()+1] = TARGET_HIGH;
}


void BPNeuralNetwork::TrainNet(int nEpochs,const std::vector<LabeledVector*>& vecLV,double eta, double dmomentum) {
	int nPoints = vecLV.size();
	// 1.train
	for (int epoch = 1; epoch <= nEpochs; epoch++) {

		double out_err;
		double hid_err;
		double sumerr = 0.0;
		for (int i = 0; i < nPoints; i++) {

			/** Set up input units and target vector on net with the i'th data **/

			LoadInputData(vecLV[i]);
			FeedForward();

			/** Run backprop, learning rate 0.3, momentum 0.3 **/
			UpdateBackward(eta, dmomentum, &out_err, &hid_err);

			sumerr += (out_err + hid_err);

		}
		cout << epoch << "\t" << sumerr << "\t";
		CalculatePerformance(vecLV, 0);
		cout << endl;

	}
}