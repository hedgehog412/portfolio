import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.*;
public class MyMatrix {
	final int myRow, myCol;
	int i,j;
	double [][] myMatrix;
	String myName;
	
	public MyMatrix(){
		myName="default";
		myRow=10;
		myCol=10;
		myMatrix=new double[myRow][myCol];
		for(i=0;i<myRow;i++){
			for(j=0;j<myRow;j++){
				myMatrix[i][j]= Math.random()*myRow*myCol;
			}
		}
	}
	public MyMatrix(int row, int col,String name){
		myName=name;
		myRow=row;
		myCol=col;
		myMatrix=new double[myRow][myCol];
		for(i=0;i<myRow;i++){
			for(j=0;j<myCol;j++){
				//myMatrix[i][j]=Math.random()*myRow*myCol;
				myMatrix[i][j]=Math.random()*10;
			}
		}
	}
	
	public String getName(){
		return myName;
	}
	
	public double getValue(int row, int col){
		return myMatrix[row-1][col-1];
	}
	
	public void changeValue(int row, int col, double value){
		myMatrix[row-1][col-1]=value;
	}
	
	public int getRow(){
		return myRow;
	}
	
	public int getCol(){
		return myCol;
	}
	
	public void initializeIdentity(){
		for(int i=0;i<myRow;i++){
			for(int j=0;j<myCol;j++){
				if(i==j) myMatrix[i][j]=1.0;
				else myMatrix[i][j]=0.0;
			}
		}
	}
	
	public void initializeRandomly(int start, int finish){
		for(i=0;i<myRow;i++){
			for(j=0;j<myCol;j++){
				myMatrix[i][j]=(Math.random()*(finish-start))+start;
			}
		}
	}//확인 바람 
	
	public void initializeFromConsole(){
		double element;
		for(i=1;i<=myRow;i++){
			for(j=1;j<=myCol;j++){
				System.out.print(myName+(i)+","+(j)+"is ");
				Scanner sc=new Scanner(System.in);
				element=sc.nextDouble();
				System.out.print(element+"\n");
				myMatrix[i-1][j-1]=element;
			}
		}
	}
	
	public MyMatrix saveMatrix(){
		MyMatrix save=new MyMatrix(myRow,myCol,"save");
		for(int i=1;i<=myRow;i++){
			for(int j=1;j<=myCol;j++){
				save.changeValue(i, j, myMatrix[i-1][j-1]);
			}
		}
		return save;
	}
	
	public MyMatrix add(MyMatrix b){
		MyMatrix result=new MyMatrix(myRow, myCol,"Result");
		if(myRow==b.getRow()&&myCol==b.getCol()){
			for(i=1;i<=myRow;i++){
				for(j=1;j<=myRow;j++){
					result.changeValue(i, j, (myMatrix[i-1][j-1]+b.getValue(i,j)));
				}
			}
		}
		
		else{
			throw new ArithmeticException("Can't add two arrays with different dimensions");
		}
		
		return result;
	}
	
	public MyMatrix subtract( MyMatrix b){
		MyMatrix result=new MyMatrix(myRow, myCol,"Result");
		if(myRow==b.getRow()&&myCol==b.getCol()){
			for(i=1;i<=myRow;i++){
				for(j=1;j<=myRow;j++){
					result.changeValue(i,  j, (myMatrix[i-1][j-1]-b.getValue(i,j)));
				}
			}
		}
		
		else{
			throw new ArithmeticException("Can't subtract two arrays with different dimensions");
		}
		
		return result;
	}
		
	static DecimalFormat df=new DecimalFormat("###.##");
	
	public static String toString(MyMatrix b){
		String[][] output=new String [b.getRow()][b.getCol()];
		for(int i=1;i<=b.getRow();i++){
			for(int j=1;j<=b.getCol();j++)
			{
				if(b.getValue(i, j)>0) df=new DecimalFormat("+###.##");
				else df=new DecimalFormat("###.##");
				output[i-1][j-1]=df.format(b.getValue(i,j));
			}
		}
		
		String result="[";
		
		for(int i=1;i<=b.getRow();i++){
			for(int j=1;j<=b.getCol();j++)
			{
				result=result.concat(output[i-1][j-1]);
				result=result.concat(" ");
			}
			if(i<b.getRow()) result=result.concat("\n");
			else result=result.concat("]");
		}
		
		return b.getName()+"\n"+b.getRow()+"X"+b.getCol()+"\n"+result;
	}
	
	public static boolean equals(MyMatrix a, MyMatrix b){
		int check=0;
		if(a.getRow()!=b.getRow()&&a.getCol()!=b.getCol()) return false;
		else{
			for(int i=1;i<=b.getRow();i++){
				for(int j=1;j<=b.getCol();j++)
				{
					if(a.getValue(i, j)!=b.getValue(i, j)){
						check=1;
						return false;
					}
				}
			}
		}
		if(check==0) return true;
		else return false;
	}
	
	public MyMatrix multiply(MyMatrix b){
		MyMatrix result=new MyMatrix(myRow,b.getCol(),"Result");
		if(myCol==b.getRow()) {
			for(int i=1;i<=myRow;i++){
				for(int j=1;j<=b.getCol();j++)
				{
					double sum=0;
					for(int x=1;x<=myCol;x++){
					sum+=myMatrix[i-1][x-1]*b.getValue(x, j);//이렇게 하는게 배열에 바로 넣는 것 보다 빠르다. 
					}
					result.changeValue(i, j, sum);
				}
			}
		}
		else throw new ArithmeticException("The matrices can not be multiplied");
		
		return result;
	}
	
	public void rowDivid(int rowNum,  double dev){
		for(int i=0;i<myCol;i++){
			myMatrix[rowNum-1][i]/=dev;
		}
	}
	
	public void rowMult(int rowNum,  double mult){
		for(int i=0;i<myCol;i++){
			myMatrix[rowNum-1][i]*=mult;
		}
	}
	
	public void rowSub(int starNum, int endNum, double times){
		for(int i=0;i<myCol;i++){
			myMatrix[endNum-1][i]-=myMatrix[starNum-1][i]*times;
		}
	}
	
	public void rowAdd(int starNum, int endNum, double times){
		for(int i=0;i<myCol;i++){
			myMatrix[endNum-1][i]+=myMatrix[starNum-1][i]*times;
		}
	}
	
	public void switchRow(int first, int sec){
		double[] save=new double[myCol];
		for(int i=0;i<myCol;i++){
			save[i]=myMatrix[first-1][i];
			myMatrix[first-1][i]=myMatrix[sec-1][i];
			myMatrix[sec-1][i]=save[i];
			
		}
	}
	
	/*
	public void gebs(MyMatrix b){
		double a;
		if(myRow==b.getRow()){
			for(int x=1;x<=myRow;x++){
					if(myMatrix[x-1][x-1]==1.0){
						for(int i=1;i<=myRow-x;i++){
							if(myMatrix[x+i-1][x-1]!=0){
								a=myMatrix[x+i-1][x-1];
								rowSub(x,x+i,a);
								b.rowSub(x,x+i,a);
							}
						}
					}
					else{
						a=myMatrix[x-1][x-1];
						rowDivid(x,a);
						b.rowDivid(x,a);
						x-=1;
					}
				}
			for(int x=myRow-1;x>=1;x--){
				for(int i=x;i<=myCol;i++){
					if(myMatrix[x][i]!=0){
					}
				}
			}
		}
		else throw new ArithmeticException("Invalid Matrix Evaluation");
	}
	//*/
	
	public MyMatrix gebs(MyMatrix b){
		double a;
		MyMatrix result=new MyMatrix(myCol,1,"Result");
		if(myRow==b.getRow()){
			for(int x=1;x<=myRow;x++){
				if(myMatrix[x-1][x-1]==1.0){
					for(int i=1;i<=myRow-x;i++){
						if(myMatrix[x+i-1][x-1]!=0.0){
							a=myMatrix[x+i-1][x-1];
							rowSub(x,x+i,a);
							b.rowSub(x, x+i, a);
						}
						for(int y=1;y<=myCol;y++){
							if(myMatrix[x-1][y-1]<0.01){
								myMatrix[x-1][y-1]=0;
							}
						}
					}
				}
				else if(myMatrix[x-1][x-1]==0){
					int r;
					for(int i=x;i<=myRow;i++){
						//*
						if(myMatrix[i-1][x-1]!=0) {
							 r=i;
							switchRow(r,x);
							b.switchRow(r,x);
							x--;
							break;
						}
						if(i==myRow){
							throw new ArithmeticException("This is a dependent matrix");
						}
					}
				}
				else{
					a=myMatrix[x-1][x-1];
					rowDivid(x,a);
					b.rowDivid(x, a);
					x--;
				}
			}
			for(int x=myRow;x>=1;x--){
				for(int y=myCol;y>x;y--){
					if(myMatrix[x-1][y-1]==0){
					}
					else{
						a=myMatrix[x-1][y-1];
						rowSub(y,x,a);
						b.rowSub(y,x,a);
					}
				}
			}
			for(int i=1;i<=myRow;i++){
				result.changeValue(i, 1, b.getValue(i,1));
			}
			return result;
		}
		else throw new ArithmeticException("Invalid Matrix Equation");
	}

	public void printResult(MyMatrix save, MyMatrix result, MyMatrix bSave){
		for(int x=myRow;x>=1;x--){
			System.out.println(((char)('Z'+1-x))+"="+df.format(myMatrix[myRow-x][0]));
		}
		System.out.println("Average Error : "+df.format(getErr(save, result, bSave))+"%");
	}
	
	public void getInverse(){
	}
	
	public double getErr(MyMatrix save, MyMatrix result, MyMatrix bSave){
		double error=0,sum=0;
		MyMatrix index=new MyMatrix(bSave.getRow(),1,"Index");
		index=save.multiply(result);
		double max,min;
		for(int i=1;i<bSave.getRow();i++){
			if(index.getValue(i, 1)>bSave.getValue(i, 1)){
				max=index.getValue(i, 1);
				min=bSave.getValue(i, 1);
			}
			else{
				max=bSave.getValue(i, 1);
				min=index.getValue(i, 1);
			}
			error=Math.abs((max-min)/max)/bSave.getRow()*100;
			sum+=error;
		}
		sum/=bSave.getRow();
		return sum;
	}
}
