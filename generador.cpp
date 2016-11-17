 #include<iostream>
 #include<vector>
 #include<cstring>
 #include<cstdlib>
 #include<cctype>
 #include<fstream>
 using namespace std;
 
 vector<string>enlaces;
 void crear_pagina(vector< string >content_doc, string name, int tam){
 	//Obtengo el contenido del post y lo pongo junto con el del articulo
 	vector< string >post;
 	char cadena[100];
 	ifstream post_tem( "template.html" );
	while( !post_tem.eof() ){
		post_tem.getline( cadena, 100 );
		if(string(cadena)=="            <article>"){
			post.push_back(string(cadena));
			for(int i=0; i<content_doc.size(); i++){
				content_doc[i] = "                " + content_doc[i];
				post.push_back(content_doc[i]);
			}
		}
		else{
			post.push_back(string(cadena));
		}
	}
	post_tem.close();
	//crear la ruta para cada entrada con el nombre de entrada intacto agregando post- antes de.
	char path[tam + 10];
	string entry = "post/";
	char* nombre_arc = new char[name.length() + 1];
	strcpy(nombre_arc, name.c_str());
	strcpy(path, entry.c_str());
	strcat(path, nombre_arc);
	enlaces.push_back(path);
	ofstream out( path );
	for( int i=0; i<post.size(); i++){
		if((post[i] != "        <nav>") && (post[i] !="            <a href="" > enlace </a>") && (post[i] != "        </nav>"))
			out << post[i] << endl ;
	}
	out.close();
 }
 
 void actualiza_blog(){
 	//leer el template
 	vector< string >out_blog;
 	char cadena[100];
 	ifstream blog( "template.html" );
	while( !blog.eof() ){
		blog.getline( cadena, 100 );
		if(strstr( cadena, "<a href=" )){
			for(int i=0; i<enlaces.size(); i++){
				string final = "<a href=";
				char comilla=34;
				final = final + comilla + enlaces[i] + comilla + " > " + enlaces[i] + "</a><br>";
				out_blog.push_back(final);
			}
		}
		else{
			out_blog.push_back(string(cadena));
		}
	}
	blog.close();
	//crear el archivo de blog.html actualizando los enlaces
	ofstream out( "blog.html" );
	for( int i=0; i<out_blog.size(); i++){
		out << out_blog[i] << endl ;
	}
	out.close();
 }
 
 int main(){
 	//genero un txt con todos los documentos html de las entradas
	system("cd entradas/ && dir *.html>docs.txt" );
	system("exit");
	//un parse para sacar los nombres de los archivos
	vector< string >name_docs;
	char cadena[100];
 	ifstream doc( "entradas/docs.txt" );
 	while( !doc.eof() ){
 		doc.getline( cadena, 100 );
 		char * pch; 
		pch = strtok( cadena, " ," ); 
		while( pch != NULL ){ 
		    if(strstr( pch, ".html" )){
		    	string cade=pch;
		    	name_docs.push_back(cade);
		    }
		    pch = strtok( NULL, " ," ); 
		} 
 	}
	doc.close();
	//tomar el contenido de cada archivo
	for(int i=0; i<name_docs.size(); i++){
		vector< string >content_doc;
		string it = name_docs[i];
		char* nombre_arc = new char[it.length() + 1];
		strcpy(nombre_arc, it.c_str());
		char path[it.length() + 10];
		string entry = "entradas/";
		strcpy(path, entry.c_str());
		strcat(path, nombre_arc);
		string func(nombre_arc);
		//Abrir archivo de entrana i
		ifstream arc( path );
		int j=0;
		//desde j=0 hasta j=nlineas del doc
		while( !arc.eof() ){
			arc.getline( cadena, 100 );
			content_doc.push_back(string(cadena));
			j++;
		}
		arc.close();
		crear_pagina(content_doc, string(nombre_arc), it.length());
		free(nombre_arc);
	}
	actualiza_blog();
	return 0;
 }
