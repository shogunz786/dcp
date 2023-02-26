fn print_vector(x:Vec<i32>){
    println!(" vector {:?}", x);
}

fn print_vectors(x:Vec<Vec<i32>>){
    for i in x.iter() {
      println!(" vector {:?}", i);
    }
}

fn two_number_sum(x:&mut Vec<i32>, t:i32) -> Vec<Vec<i32>> {
    x.sort();
    let mut l = 0;
    let mut r = x.len()-1;
    let mut res: Vec<Vec<i32>> = vec![];
    
    while l < r {
        let q = x[l]+x[r];
        if q<t {
            l=l+1;
        }else if q>t {
            r=r-1;
        }else{
            res.push(vec![]);
            let len = res.len()-1;
            res[len].push(x[l]);
            res[len].push(x[r]);
            l=l+1;
            r=r-1;
        }
    }
    return res;
}

fn main() {
    let mut v =vec![3,5,-4,8,11,1,-1,6];
    print_vector(v.clone());
    let res:Vec<Vec<i32>> = two_number_sum(&mut v, 10);
    print_vectors(res.clone());
}
