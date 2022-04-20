import numpy as np 
import torch 
import os 
import argparse

def read_file(input_file):

    # os.system('5830hw/matmul 5830hw/sample1.in 5830hw/sample1.temp')

    sample = open(input_file, 'r').read()

    N, D, K, A, B, C, M, L1, L2, L3 = [int(s) for s in sample.split('\n')[0].split()]


    # generate X 
    X = np.zeros(N * D)

    for i in range(K): 
        X[i] = int(sample.split('\n')[i+1].split()[0])

    for i in range(K, N*D): 
        
        X[i] = (A * X[i - 1] + (B * X[i - 2] + C)) % M 


    X2, X1 = X[-2], X[-1]



    return N, D, K, A, B, C, M, L1, L2, L3, X[:L1], X2, X1


def read_file_with_help(input_file): 

    c_file = os.path.join(os.path.dirname(input_file), 'matmul')
    temp_file = input_file + 'extra'

    sample = open(input_file, 'r').read()
    N, D, K, A, B, C, M, L1, L2, L3 = [int(s) for s in sample.split('\n')[0].split()]

    os.system('%s %s %s' % (c_file, input_file, temp_file))

        
    temp_file = input_file + 'extra'

    tempX = open(temp_file, 'r').read()

    tempX = tempX.split('\n')

    X2 = int(tempX[0])
    X1 = int(tempX[1])


    X = [s.split() for s in tempX[2:-1]]

    X  = np.array(X, dtype = np.int32)

    return N, D, K, A, B, C, M, L1, L2, L3, X, X2, X1


def generate_parameters(x0, x1, A, B, C, M, size): 

    X = np.zeros(size)
    X[0] = x0
    X[1] = x1

    for i in range(2, size): 
        X[i] = (A * X[i - 1] + (B * X[i - 2] + C)) % M 

    return X 


def activation(X, D, cutoff):
    cutoff = int(cutoff)
    
    for i in range(D): 
        if (X[i] < cutoff):
            X[i] = 0
    
    return X 

def forward(X, D, P, L, out):

    for i in range(L): 
        out[i] = 0
    for i in range(D): 
        for j in range(L): 
            out[j] = (out[j] + X[j] * P[j*D + i]) % M

    return out 


def modulo_matmul(X, W, D):

    w = np.reshape(W, (-1, D))
    x = np.reshape(X[:w.shape[0]], (-1, 1))

    return np.sum( (x * w) % M, axis = 1) % M 

def torch_modulo_matmul(X, W, D, M):

    w = torch.reshape(W, (-1, D))
    x = torch.reshape(X[:w.shape[0]], (-1, 1))

    y = (x * w) % M

    return torch.sum(y, axis = 1) % M

def torch_activation(X, cutoff):
    return torch.nn.Threshold(int(cutoff) - 0.5, 0)(X)


def main(input_file, output_file):

    N, D, K, A, B, C, M, L1, L2, L3, X, X2, X1  = read_file_with_help(input_file)


    P1 = generate_parameters(X2, X1, A, B, C, M, D * L1)
    P2 = generate_parameters(P1[D * L1 - 2], P1[D * L1 - 1], A, B, C, M, L1 * L2)
    P3 = generate_parameters(P2[L1 * L2 - 2], P2[L1 * L2 - 1], A, B, C, M, L2 * L3)

    P1, P2, P3 = [torch.tensor(P, dtype = torch.int32).cuda() for P in [P1, P2, P3]]

    X = torch.tensor(X, dtype = torch.int32).cuda() 

    results = [] 
    for i in range(N):

   
        x = torch_modulo_matmul(X[i], P1, D, M)
        x = torch_activation(x, M/2)

        x = torch_modulo_matmul(x, P2, L1, M)
        x = torch_activation(x, M/2)
       
        x = torch_modulo_matmul(x, P3, L2, M)

        res = torch.argmax(x).cpu();

        res = int(res.numpy())
        results.append(res)
        # print(res, answers[i])

    _ = open(output_file, 'w').write('\n'.join([str(res) for res in results]))

    
if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument('read_path', type = str)
    parser.add_argument('write_path', type = str)
    args = parser.parse_args()

    main(args.read_path, args.write_path)