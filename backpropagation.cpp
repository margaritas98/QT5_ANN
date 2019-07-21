#include "backpropagation.h"


#define sqr(x)	((x) * (x))

//FILE *out;

//#define LEARN_RATE		0.2

#define RAND_WEIGHT	( ((float)rand() / (float)RAND_MAX) - 0.5)

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)      (int)((float)(x)*rand()/(RAND_MAX+1.0))


///////////////////////////////////////////////////////////////////

Backpropagation::Backpropagation()
{
   initialise();
}  //untouched

void Backpropagation::initialise()
{
    err=0;
    sample=0;
    iterations=0;
    sum = 0;


    /* Seed the random number generator */
    srand((time(NULL) ) );

    assignRandomWeights();
}  //untouched

double Backpropagation::getError_SSE(){
    return err2;
}  //untouched, just change from err to err2


////for debugging - very good
//void Backpropagation::saveWeights(){
//    int out, hid, inp;

//    QFile file3("weights.txt");
//    file3.open(QIODevice::WriteOnly | QIODevice::Text);

//    QTextStream out3(&file3);

//    char tempBuffer3[80];
//    QByteArray temp3;

//    //----------------------------------------------
//    // weights
//    //

//    qDebug() << "updating weights...";
//    qDebug() << "OUTPUT_NEURONS = " << OUTPUT_NEURONS;
//    qDebug() << "HIDDEN_NEURONS = " << HIDDEN_NEURONS;
//    qDebug() << "INPUT_NEURONS = " << INPUT_NEURONS;

//    // Update the weights for the output layer (step 4 for output cell)
//    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
//      temp3.clear();
//      for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//          //---save------------------------------------


//            ::sprintf(tempBuffer3,"who[%d][%d]=%f , ",hid,out,who[hid][out]);
//            temp3.append(tempBuffer3);

//          //---------------------------------------
//      }

//      // Update the Bias
//      //---save------------------------------------
//        ::sprintf(tempBuffer3,"who[%d][%d]=%f",HIDDEN_NEURONS,out,who[HIDDEN_NEURONS][out]);
//        temp3.append(tempBuffer3);
//        temp3.append("\n");
//        out3 << temp3;

//      //---------------------------------------

//    }

//    // Update the weights for the hidden layer (step 4 for hidden cell)
//    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//      temp3.clear();
//      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {

//        //---save------------------------------------


//          ::sprintf(tempBuffer3,"wih[%d][%d]=%f , ",inp,hid,wih[inp][hid]);
//          temp3.append(tempBuffer3);

//        //---------------------------------------
//      }

//      // Update the Bias
//      //---save------------------------------------
//        ::sprintf(tempBuffer3,"wih[%d][%d]=%f",INPUT_NEURONS,hid,wih[INPUT_NEURONS][hid]);
//        temp3.append(tempBuffer3);
//        temp3.append("\n");
//        out3 << temp3;

//      //---------------------------------------

//    }

//    //----------------------------------------------


//    file3.close();
//    qDebug() << "Weights saved to file.";
//}  //untouched


void Backpropagation::saveWeights(QString fileName){
    int out, hid, inp;

    QFile file3(fileName);
    file3.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out3(&file3);

    char tempBuffer3[80];
    QByteArray temp3;

    //----------------------------------------------
    // weights
    //

    qDebug() << "updating weights...";
    qDebug() << "OUTPUT_NEURONS = " << OUTPUT_NEURONS;
    qDebug() << "HIDDEN_NEURONS = " << HIDDEN_NEURONS;
    qDebug() << "INPUT_NEURONS = " << INPUT_NEURONS;

    // Update the weights for the output layer (step 4 for output cell)
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      temp3.clear();
      for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
          //---save------------------------------------


            ::sprintf(tempBuffer3,"%f,",who[hid][out]);
            temp3.append(tempBuffer3);

          //---------------------------------------
      }

      // Update the Bias
      //---save------------------------------------
        ::sprintf(tempBuffer3,"%f",who[HIDDEN_NEURONS][out]);
        temp3.append(tempBuffer3);
        temp3.append("\n");
        out3 << temp3;

      //---------------------------------------

    }


    //update the weights for hidden layer 2  //new
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      temp3.clear();
      for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {

        //---save------------------------------------


          ::sprintf(tempBuffer3,"%f,",whh[hid2][hid]);
          temp3.append(tempBuffer3);

        //---------------------------------------
      }

      // Update the Bias
      //---save------------------------------------
        ::sprintf(tempBuffer3,"%f",whh[HIDDEN_NEURONS][hid]);
        temp3.append(tempBuffer3);
        temp3.append("\n");
        out3 << temp3;

      //---------------------------------------

    }

    //----------------------------------------------


    // Update the weights for the hidden layer (step 4 for hidden cell)
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      temp3.clear();
      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {

        //---save------------------------------------


          ::sprintf(tempBuffer3,"%f,",wih[inp][hid]);
          temp3.append(tempBuffer3);

        //---------------------------------------
      }

      // Update the Bias
      //---save------------------------------------
        ::sprintf(tempBuffer3,"%f",wih[INPUT_NEURONS][hid]);
        temp3.append(tempBuffer3);
        temp3.append("\n");
        out3 << temp3;

      //---------------------------------------

    }

    //----------------------------------------------


    file3.close();
    qDebug() << "Weights saved to file.";
}


void Backpropagation::loadWeights(QString fileName){
    int out, hid, inp;
    qDebug() << fileName;
    QFile file3(fileName);
    file3.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!file3.exists()){
        qDebug() << "Backpropagation::loadWeights-file does not exist!";
        return;
    }

    QTextStream in(&file3);

    char tChar;
    //char tempBuffer3[80];
    QByteArray temp3;

    //----------------------------------------------
    // weights
    //

    QString strLine;
    //QTextStream streamLine;

    qDebug() << "loading weights...";
    qDebug() << "OUTPUT_NEURONS = " << OUTPUT_NEURONS;
    qDebug() << "HIDDEN_NEURONS = " << HIDDEN_NEURONS;
    qDebug() << "INPUT_NEURONS = " << INPUT_NEURONS;

    // Update the weights for the output layer (step 4 for output cell)
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      strLine = in.readLine();
      QTextStream streamLine(&strLine);

      for (hid = 0 ; hid <= HIDDEN_NEURONS ; hid++) {  //<=????
          //---load------------------------------------

            if(hid != HIDDEN_NEURONS-1){
               streamLine >> who[hid][out] >> tChar;
            } else {
               streamLine >> who[hid][out];
            }


          //---------------------------------------
      }

      // Update the Bias
      //---load------------------------------------

        streamLine >> who[HIDDEN_NEURONS][out];// >> tChar;


      //---------------------------------------

    }

    //update the weights for the hidden layer 2  //new
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
        strLine = in.readLine();
        QTextStream streamLine(&strLine);

      for (int hid2 = 0 ; hid2 <= HIDDEN_NEURONS ; hid2++) {

        //---load------------------------------------
          if(hid2 != HIDDEN_NEURONS-1){
             streamLine >> whh[hid2][hid] >> tChar;
          } else {
             streamLine >> whh[hid2][hid];
          }

        //---------------------------------------
      }

      // Update the Bias
      //---load------------------------------------
        streamLine >> whh[HIDDEN_NEURONS][hid]; // >> tChar;

      //---------------------------------------

    }

    //----------------------------------------------


    /* Update the weights for the hidden layer (step 4 for hidden cell) */
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
        strLine = in.readLine();
        QTextStream streamLine(&strLine);

      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {

        //---load------------------------------------
          //if(hid != INPUT_NEURONS){
             streamLine >> wih[inp][hid] >> tChar;
          //} else {
          //   streamLine >> wih[inp][hid];
          //}

        //---------------------------------------
      }

      // Update the Bias
      //---load------------------------------------
        streamLine >> wih[INPUT_NEURONS][hid]; // >> tChar;

      //---------------------------------------

    }

    //----------------------------------------------


    file3.close();
    qDebug() << "Weights loaded.";
/*
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      for (hid = 0 ; hid <= HIDDEN_NEURONS ; hid++) {
           qDebug() << who[hid][out];
      }
     }

    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
             qDebug() << wih[inp][hid];
          }
      }
*/
}  //load weight, changed in to streamline at hidden layer

int Backpropagation::action( double *vector )
{
  int index, sel;
  double max;

  sel = 0;
  max = vector[sel];

  for (index = 1 ; index < OUTPUT_NEURONS ; index++) {
    if (vector[index] > max) {
      max = vector[index]; sel = index;
    }
  }

  return( sel );
}

double* Backpropagation::testNetwork(LetterStructure testPattern2){
    //retrieve input patterns
    for(int j=0; j < INPUT_NEURONS; j++){
       inputs[j] = testPattern2.f[j];
       if (isNormalise) inputs[j] = 1.0 * inputs[j] / 15.0 - 0.5;
//       qDebug() << "f[" << j << "] = " << inputs[j];
    }

    for(int i=0; i < OUTPUT_NEURONS; i++){
        target[i] = testPattern2.outputs[i];
//        qDebug() << "desired_output[" << i << "] = " << target[i];
    }

    feedForward();


//    for (int out = 0 ; out < OUTPUT_NEURONS ; out++) {

//       qDebug() << "Output[" << out << "] = " << actual[out];

//    }

    return actual;


}


double Backpropagation::trainNetwork(int NUMBER_OF_DESIRED_EPOCHS)
{
    if(!patternsLoadedFromFile) {
        qDebug() << "unable to train network, no training patterns loaded.";
        return -999.99;
    }
    double accumulatedErr=0.0;
    iterations=0;
    int epochs=0;
    err = 0.0;
    while (1) {

        if (sample == NUMBER_OF_TRAINING_PATTERNS) {
            sample = 0;
            err = 0.0;
            epochs++;
        }

        if(epochs >= NUMBER_OF_DESIRED_EPOCHS) {

            break;
        }

        //retrieve input patterns
        for(int j=0; j < INPUT_NEURONS; j++){
           inputs[j] = letters[sample].f[j];
           if (isNormalise) inputs[j] = 1.0 * inputs[j] / 15.0 - 0.5;
        }

        for(int i=0; i < OUTPUT_NEURONS; i++){
            target[i] = letters[sample].outputs[i];
        }

        feedForward();

        /* need to iterate through all ... */

        //err = 0.0;
        for (int k = 0 ; k < OUTPUT_NEURONS ; k++) {

          err += sqr( (letters[sample].outputs[k] - actual[k]) );
        }

        err = 0.5 * err;
        err2 = err;
        accumulatedErr = accumulatedErr + err;

        backPropagate();
        sample ++;

      }
      qDebug() << "training complete.";
      return accumulatedErr;
}


/*
 *  assignRandomWeights()
 *
 *  Assign a set of random weights to the network.
 *
 */

void Backpropagation::assignRandomWeights( void ) //random on bias as well (weights)
{
  int hid, inp, out;

  for (inp = 0 ; inp < INPUT_NEURONS+1 ; inp++) {
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      wih[inp][hid] = RAND_WEIGHT;
    }
  }


  for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS+1 ; hid2++) {  //new
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      whh[hid2][hid] = RAND_WEIGHT;
    }
  }

  for (hid = 0 ; hid < HIDDEN_NEURONS+1 ; hid++) {
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      who[hid][out] = RAND_WEIGHT;
    }
  }

}


/*
 *  sigmoid()
 *
 *  Calculate and return the sigmoid of the val argument.
 *
 */

double Backpropagation::sigmoid( double val )
{
  return (1.0 / (1.0 + exp(-val)));
}


/*
 *  sigmoidDerivative()
 *
 *  Calculate and return the derivative of the sigmoid for the val argument.
 *
 */

double Backpropagation::sigmoidDerivative( double val )
{
  return ( val * (1.0 - val) );
}

/*
 *  tanh()
 *
 *  Calculate and return the tanh of the val argument.
 *
 */

double Backpropagation::tanh( double val )
{
    return 1.0 * (exp(val) - exp(-1.0 * val)) / (exp(val) + exp(-1.0 * val) );
}


/*
 *  tanhDerivative()
 *
 *  Calculate and return the derivative of the tanh for the val argument.
 *
 */

double Backpropagation::tanhDerivative( double val )
{
    return 1.0 - tanh(val) * tanh(val);
}

/*
 *  relu()
 *
 *  Calculate and return the relu of the val argument.
 *
 */
double Backpropagation::relu( double val )
{
    return fmax(0.0, val);
}

/*
 *  reluDerivative()
 *
 *  Calculate and return the derivative of the relu for the val argument.
 *
 */
double Backpropagation::reluDerivative( double val )
{
    if (val > 0)
        return 1.0;
    else
        return 0.0;
} 

/*
 *  feedForward()
 *
 *  Feedforward the inputs of the neural network to the outputs.
 *
 */
void Backpropagation::feedForward( )
{
  int inp, hid, out;
  double sum, sum2 = 0.0;

  // Calculate input to hidden layer
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    sum = 0.0;
    for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
      sum += inputs[inp] * wih[inp][hid];
    }

    // Add in Bias
    sum += wih[INPUT_NEURONS][hid];

    if (activation_function == RELU)
        hidden[hid] = relu( sum );
    else if (activation_function == TANH)
        hidden[hid] = tanh( sum );
    else if (activation_function == SIGMOID)
        hidden[hid] = sigmoid( sum );

  }

  // calc the hidden1 to hidden2
  for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {

    sum = 0.0;
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      sum += hidden[hid] * whh[hid][hid2];
    }

    // Add in Bias
    sum += whh[HIDDEN_NEURONS][hid2];

    if (activation_function == RELU)
        hidden2[hid2] = relu( sum );
    else if (activation_function == TANH)
        hidden2[hid2] = tanh( sum );
    else if (activation_function == SIGMOID)
        hidden2[hid2] = sigmoid( sum );

  }

  sum2 = 0.0;
  // Calculate the hidden to output layer // //Use softmax units at the output layer
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {

    sum = 0.0;
    for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {
      sum += hidden2[hid2] * who[hid2][out];
    }

    // Add in Bias
    sum += who[HIDDEN_NEURONS][out];

    // start softmax units, don't need relu, tanh, or sigmoid in this layer
    actual[out] = exp ( sum  );

    sum2 += actual[out];
    //actual[out] = sigmoid( sum );
  }

  for (int i=0; i<OUTPUT_NEURONS; i++) {  // every output is exp() / total_exp
    actual[i] = 1.0 * actual[i] / sum2;
  }
}



/*
 *  backPropagate()
 *
 *  Backpropagate the error through the network.
 *
 */
void Backpropagation::backPropagate( void ) //gradient ascent not work well, change to descent
{
  int inp, hid, out;

  // Calculate the output layer error (step 3 for output cell)   //erro
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      erro[out] = actual[out] - target[out];
      if (activation_function == RELU)
        erro[out] *= reluDerivative( actual[out] );
      else if  (activation_function == TANH)
          erro[out] *= tanhDerivative( actual[out] );
      else if  (activation_function == SIGMOID)
          erro[out] *= sigmoidDerivative( actual[out] );
          //erro[out] = (target[out] - actual[out]) * sigmoidDerivative( actual[out] );
  }

    // Calculate the hidden layer2 error (step 3 for hidden cell)   //errh2
    for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {

      errh2[hid2] = 0.0;
      for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
        errh2[hid2] += erro[out] * who[hid2][out];
      }

      if (activation_function == RELU)
          errh2[hid2] *= reluDerivative( hidden2[hid2] );
      else if (activation_function == TANH)
          errh2[hid2] *= tanhDerivative( hidden2[hid2] );
      else if (activation_function == SIGMOID)
          errh2[hid2] *= sigmoidDerivative( hidden2[hid2] );

    }

    // Calculate the hidden layer1 error (step 3 for hidden cell)  //errh
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

      errh[hid] = 0.0;
      for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {
        errh[hid] += errh2[hid2] * whh[hid][hid2];
      }

      if (activation_function == RELU)
          errh[hid] *= reluDerivative( hidden[hid] );
      else if (activation_function == TANH)
          errh[hid] *= tanhDerivative( hidden[hid] );
      else if (activation_function == SIGMOID)
          errh[hid] *= sigmoidDerivative( hidden[hid] );

    }

  // Update the weights for the output layer (step 4 for output cell)   //who: erro, hidden2
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {

    for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {
      who[hid2][out] -= (LEARNING_RATE * erro[out] * hidden2[hid2]);
    }

    // Update the Bias
    who[HIDDEN_NEURONS][out] -= (LEARNING_RATE * erro[out] * 1.0);

  }

  // Update the weights for the hidden layer2 (step 4 for hidden cell)   //whh: errh2, hidden
  for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {

    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      whh[hid][hid2] -= (LEARNING_RATE * errh2[hid2] * hidden[hid]);
    }

    // Update the Bias
    whh[HIDDEN_NEURONS][hid2] -= (LEARNING_RATE * errh2[hid2] *1.0);

  }

  // Update the weights for the hidden layer1 (step 4 for hidden cell)   //wih: errh, inputs
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
      wih[inp][hid] -= (LEARNING_RATE * errh[hid] * inputs[inp]);
    }

    // Update the Bias
    wih[INPUT_NEURONS][hid] -= (LEARNING_RATE * errh[hid]);

  }

}


void Backpropagation::feedForward2( )
{
  int inp, hid, out;
  double sum, sum2 = 0.0;

  /* Calculate input to hidden layer */
  //Experiment with Relu and tanh as the activation functions of your hidden units, include sigmoid with lecture code
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    sum = 0.0;
    for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
      sum += inputs[inp] * wih[inp][hid];
    }

    /* Add in Bias */
    sum += wih[INPUT_NEURONS][hid];

    if (activation_function == RELU)
        hidden[hid] = relu( sum );
    else if (activation_function == TANH)
        hidden[hid] = tanh( sum );
    else if (activation_function == SIGMOID)
        hidden[hid] = sigmoid( sum );

  } //good

  sum2 = 0.0; //new add

  /* Calculate the hidden to output layer */ //Use softmax units at the output layer
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {

    sum = 0.0;
    for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {
      sum += hidden[hid2] * who[hid2][out];
    }

    /* Add in Bias */
    sum += who[HIDDEN_NEURONS][out];
    // start softmax, so dont need relu, tanh, sigmoid in this layer
    actual[out] = exp ( sum  );

    //actual[out] = sigmoid( sum );

    sum2 += actual[out];
  }

  for (int i=0; i<OUTPUT_NEURONS; i++) {  // every output is exp() / total_exp
    actual[i] = 1.0 * actual[i] / sum2;
  } //good


}  // google and learn softmax in output layer, test later, maybe ok


/*
 *  backPropagate()
 *
 *  Backpropagate the error through the network.
 *
 */

void Backpropagation::backPropagate2( void ) //gradient ascent not work well, change to descent
{
  int inp, hid, out;
//1
  /* Calculate the output layer error (step 3 for output cell) */  //erro
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      erro[out] = actual[out] - target[out];
      if (activation_function == RELU)
        erro[out] *= reluDerivative( actual[out] );
      else if  (activation_function == TANH)
          erro[out] *= tanhDerivative( actual[out] );
      else if  (activation_function == SIGMOID)
          erro[out] *= sigmoidDerivative( actual[out] );
          //erro[out] = (target[out] - actual[out]) * sigmoidDerivative( actual[out] );
  }  //good

  /* Calculate the hidden layer2 error (step 3 for hidden cell) */  //old, should be ok, check again, //errh2
  for (int hid2 = 0 ; hid2 < HIDDEN_NEURONS ; hid2++) {

    errh[hid2] = 0.0;
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      errh[hid2] += erro[out] * who[hid2][out];
    }

    if (activation_function == RELU) {
        errh[hid2] *= reluDerivative( hidden[hid2] );
    }
    else if (activation_function == TANH) {
        errh[hid2] *= tanhDerivative( hidden[hid2] );
    }
    else if (activation_function == SIGMOID) {
        errh[hid2] *= sigmoidDerivative( hidden[hid2] );
    }

  } //good


  /* Update the weights for the output layer (step 4 for output cell) */  //who , erro, hidden2
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {

    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      who[hid][out] -= (LEARNING_RATE * erro[out] * hidden[hid]);
    }

    /* Update the Bias */
    who[HIDDEN_NEURONS][out] -= (LEARNING_RATE * erro[out] * 1.0);

  }  //good


//2


  /* Update the weights for the hidden layer1 (step 4 for hidden cell) */  //old, wih, errh, inputs
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
      wih[inp][hid] -= (LEARNING_RATE * errh[hid] * inputs[inp]);
    }

    /* Update the Bias */
    wih[INPUT_NEURONS][hid] -= (LEARNING_RATE * errh[hid] * 1.0);

  } //good



}  //change a bit

