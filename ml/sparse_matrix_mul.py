def sparse_matrix_multiplication(matrix_a, matrix_b):
    if len(matrix_a[0]) != len(matrix_b):
       return [[]]
    #len(matrix_a)*len(matrix_a[0])
    sparse_a = get_sparse_keys(matrix_a)

    #len(matrix_b)*len(matrix_b[0])
    sparse_b = get_sparse_keys(matrix_b)

    #len(matrix_a)*len(matrix_b[0])
    matrix_c = [[0]*len(matrix_b[0]) for _ in range(len(matrix_a))]

    for (i,k) in sparse_a.keys():
        for j in range(len(matrix_b[0])):
           #if k,j is a sparse key 
           if (k,j) in sparse_b.keys():
               matrix_c[i][j] += sparse_a[(i, k)]*sparse_b[(k,j)]
            
    return matrix_c

#build map of element location and element value
def get_sparse_keys(matrix):
    sparse_keys={}
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] != 0:
                sparse_keys[(i,j)]=matrix[i][j]
    return sparse_keys
