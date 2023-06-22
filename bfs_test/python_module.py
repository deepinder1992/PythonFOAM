import traceback
import sys
import matplotlib.pyplot as plt
import numpy as np
np.random.seed(10)
import tensorflow as tf
print("imported1")
#tf.disable_v2_behavior()
print("imported2")
tf.random.set_seed(10)
from tensorflow.keras.layers import Input, Dense, Concatenate
from tensorflow.keras import optimizers, models, regularizers
from tensorflow.keras import backend as K
from tensorflow.keras.callbacks import ModelCheckpoint, EarlyStopping
from tensorflow.keras.models import load_model, Sequential, Model
from tensorflow.keras.optimizers import Adam, SGD, RMSprop 
import tensorflow.keras.backend as kb  
#tf.compat.v1.enable_eager_execution()
#print(tf.executing_eagerly()) 


initializer = tf.keras.initializers.ones()
u_inputs = Input(shape=(900))
x = Dense(10, activation='relu', dtype='float64',use_bias =True)(u_inputs)
x = Dense(10, activation='relu', dtype='float64',use_bias =False)(x) 
x = Dense(10, activation='relu', dtype='float64',use_bias =False)(x)
x = Dense(10, activation='relu', dtype='float64',use_bias =False)(x)
z = Dense(900, activation='linear', dtype='float64',use_bias =True)(x)
model = Model(inputs=[u_inputs],outputs=z)
model.summary()
vel =  np.array([1])
loss =np.array([]) 
counter = 0
iters = 300
lr1 = 1e-2
def velocity_inp(array,array_2):
    array = tf.Variable(array,dtype = tf.float64)
    array_2 = tf.Variable(array_2,dtype = tf.float64)
    array_2 = tf.reshape(array_2,(5,900,1))
    input1= np.loadtxt('U.csv',delimiter=',')/50
    input_1 = input1.T
    
    print(input1.shape)
    
    global lr1, counter,inv,iters
    if counter%20==0 or counter<=20:
        inv = tf.linalg.inv(array)
        print("**********************************************************************************************************************************************")
    if counter%40==0 and counter>0:
        lr1 = lr1/3
        print(lr1)

    if counter%20==0 and counter>0:
        iters-=10
        if iters<=50:
           iters = 50
        print("**********************************************************************************************************************************************")
    counter+=1
    lr = lr1
    for iter in range(0,iters):
        for i in range(0,1):
            with tf.GradientTape() as tape:
                output_1 = model([input_1],training = True)
                output_2 = tf.reshape(output_1*0.0001,(5,900,1))
                loss1 = tf.subtract(array_2,tf.matmul(array,output_2))
                loss2 = tf.math.reduce_mean(tf.abs(tf.matmul(inv,loss1)),axis = 0,keepdims = True)
                np.savetxt('loss2.csv',loss2[:,:,0].numpy().T,delimiter = ',')
                loss2 = tf.transpose(loss2[:,:,0])
                if iter%10==0:
                	print(iter, "....",np.mean(tf.abs(loss2).numpy()),"....",lr)
                grads = tape.gradient(loss2, model.trainable_variables)
                Adam(learning_rate= lr).apply_gradients(zip(grads, model.trainable_variables))
            if iter%50==0 and iter>0:
               lr = lr/3
            if lr<1e-9:
               lr = 1e-9
    nuTilda1 = output_2
    chi = nuTilda1/(1.5e-5)
    con = 7.1
    chi3 = chi**3
    con3 = con**3
    fv = chi3/(con3+chi3)
    nut1 = fv*nuTilda1 
    model.save("aero_trained_full")
    np.savetxt("nuu.csv", nuTilda1[:,:,0].numpy().T,delimiter =',')
    return nut1[:,:,0].numpy().T,nuTilda1[:,:,0].numpy().T

def velocity_save(array):
     np.savetxt('U.csv',array[:,:,0].T, delimiter=',')
     np.savetxt('V.csv',array[:,:,1].T, delimiter=',')
     return 0

     
     



















