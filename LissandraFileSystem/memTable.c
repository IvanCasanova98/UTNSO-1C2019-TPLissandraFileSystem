#include "memTable.h"


long long SELECT(char *nombre_tabla, uint16_t valor_key_buscado){

//	t_KeyValue_Timestamp keyValue_timestamp;

	long long timestampMayorEncontrado;
	ptr_nodo_tabla TablaInicial; //tengo dudas aca. quiero apuntar al primer elemento

	ptr_nodo_tabla TablaBuscada = BuscarLaTabla(TablaInicial,nombre_tabla);


	if (TablaBuscada == NULL)
	{
		printf("No se encontro tabla.\n");
	}
	else
	{
		timestampMayorEncontrado = Buscar_KeyValue_conMayor_TimeStamp(TablaBuscada,valor_key_buscado);
	}

	return timestampMayorEncontrado;

}

void INSERT(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp){


	  ptr_nodo_registro registroIngresado;
	  registroIngresado = (ptr_nodo_registro) malloc (sizeof(nodo_registro_tmp));
	  registroIngresado = CargarRegistrosAUnNodo(valor_key,value,timestamp);

	  ptr_nodo_tabla TablaInicial; //tengo dudas aca. quiero apuntar al primer elemento
	  ptr_nodo_tabla TablaDeseada;

	  TablaDeseada= BuscarLaTabla(TablaInicial,nombre_tabla);

	  if ( TablaDeseada == NULL) //No Existe la tabla ingresada. Habra que crearla.
	  {
	  	  ptr_nodo_tabla nuevaTabla;
	  	  nuevaTabla = (ptr_nodo_tabla) malloc(sizeof(nodo_tabla_tmp));

	  	  nuevaTabla = CrearTabla(nombre_tabla);
	  	  InsertarTablaAlFinal(registroIngresado,TablaInicial, nuevaTabla);
	  }
	  else
	  {
		  InsertarleRegistroATablaAlFinal(registroIngresado,TablaDeseada);
	  }



}

//*********************************FUNCION PARA TESTEAR

void BorrarRegistrosDeTabla(ptr_nodo_tabla TablaABorrar)
{
	ptr_nodo_registro RegistroAuxiliarActual;
	ptr_nodo_registro RegistroAuxiliarSgte;

	RegistroAuxiliarActual = TablaABorrar->sgte_registro;
	RegistroAuxiliarSgte = RegistroAuxiliarActual->sgte_registro;

	TablaABorrar->sgte_registro=NULL;

	while(RegistroAuxiliarActual != NULL)
	{
		free(RegistroAuxiliarActual);
		RegistroAuxiliarActual = RegistroAuxiliarSgte;
		if(RegistroAuxiliarActual == NULL)
		{
			break; //Breakeo aca porque no quiero que le asigne nada al RegistroAuxiliarSgte.
		}
		else RegistroAuxiliarSgte = RegistroAuxiliarSgte->sgte_registro;
	}
}

void imprimirRegistrosVALUEDe(char *nombre_tabla)
{
	ptr_nodo_tabla TablaInicial;
	ptr_nodo_tabla TablaDeseada;
	TablaDeseada= BuscarLaTabla(TablaInicial, nombre_tabla);

	ptr_nodo_registro RegistroAuxiliar;

	TablaDeseada->sgte_registro = RegistroAuxiliar;
	printf("%s\n",RegistroAuxiliar->value);
	while(RegistroAuxiliar->sgte_registro != NULL)
	{
		RegistroAuxiliar = RegistroAuxiliar->sgte_registro;
		printf("%s\n",RegistroAuxiliar->value);
	}

}
//*****************************************************

long long Buscar_KeyValue_conMayor_TimeStamp(ptr_nodo_tabla TablaBuscada,uint16_t valor_key_buscado)
{

	long long timeStampMax = 0;
	ptr_nodo_registro RegistroAuxiliar;

	RegistroAuxiliar = TablaBuscada->sgte_registro; //Pongo como registro auxiliar el primer registro.

	while(RegistroAuxiliar != NULL)
	{
		if( (RegistroAuxiliar->valor_key == valor_key_buscado) &&
		    (RegistroAuxiliar->timestamp > timeStampMax))
		{
			timeStampMax = RegistroAuxiliar->timestamp;
		}
		RegistroAuxiliar = RegistroAuxiliar->sgte_registro;
	}

	if (timeStampMax== 0) printf("\nNo Se Encontro ningun registro del KEY-VALUE deseado.\n");
	return timeStampMax;
}


void InsertarleRegistroATablaAlFinal(ptr_nodo_registro nuevoRegistro, ptr_nodo_tabla TablaDeseada)
{
	ptr_nodo_registro RegistroAuxiliar;

	TablaDeseada->sgte_registro = RegistroAuxiliar;
	while(RegistroAuxiliar->sgte_registro != NULL)
	{
		RegistroAuxiliar = RegistroAuxiliar->sgte_registro;
	}
	RegistroAuxiliar->sgte_registro = nuevoRegistro;

}

ptr_nodo_tabla BuscarLaTabla(ptr_nodo_tabla TablaInicial, char *nombre_tabla)
{
	ptr_nodo_tabla Auxiliar;
	Auxiliar = TablaInicial;
	int loEncontro = 0;

	while((Auxiliar != NULL) && !loEncontro)
	{
		if(strcmp(Auxiliar->nombre_tabla,nombre_tabla) == 0)
		{
			loEncontro = 1;
		}
		else
		{
			Auxiliar = Auxiliar->sgte_tabla;
		}
	}

	if(loEncontro)
	{
		return Auxiliar;
	}
	else
	{
		return NULL;
	}

}

ptr_nodo_registro CargarRegistrosAUnNodo(uint16_t valor_key, char *value, long long timestamp)
{
	ptr_nodo_registro registro;

	registro = (ptr_nodo_registro) malloc (sizeof(nodo_registro_tmp));

	registro->valor_key = valor_key;
	strcpy(registro->value,value);
	registro->timestamp = timestamp;
	registro->sgte_registro = NULL;

	return registro;
}

ptr_nodo_tabla CrearTabla(char *nombre_tabla)
{

	ptr_nodo_tabla NuevaTabla;

	NuevaTabla = (ptr_nodo_tabla) malloc(sizeof(nodo_tabla_tmp));
	strcpy(NuevaTabla->nombre_tabla,nombre_tabla);
	NuevaTabla->sgte_registro = NULL;
	NuevaTabla->sgte_tabla = NULL;
	return NuevaTabla;
}

void InsertarTablaAlFinal(ptr_nodo_registro registroDeLaNuevaLista,ptr_nodo_tabla TablaInicial,ptr_nodo_tabla TablaAInsertar)
{
	ptr_nodo_tabla Auxiliar = TablaInicial;

//	TablaAInsertar = malloc (sizeof(nodo_tabla_tmp));
	if(TablaInicial->sgte_tabla == NULL)
	{
		TablaInicial->sgte_tabla = TablaAInsertar;
	}
	else
	{
		while(Auxiliar->sgte_tabla != NULL)
		{
			Auxiliar= Auxiliar->sgte_tabla;
		}
		Auxiliar->sgte_tabla = TablaAInsertar;
	}

	TablaAInsertar->sgte_registro = registroDeLaNuevaLista;


}

//void InsertarleRegistroATablaAlFinal(char *nombre_tabla,ptr_nodo_registro nuevoRegistro, ptr_nodo_tabla TablaInicial)
//{
//	//Primero lo busca con la certeza de que existe.
//
//		ptr_nodo_tabla TablaAuxiliar;
//		TablaAuxiliar = TablaInicial;
//
//		while(TablaAuxiliar!= NULL)
//		{
//			if(strcmp(TablaAuxiliar->nombre_tabla,nombre_tabla,0) == 0)
//			{
//				break;
//			}
//			TablaAuxiliar = TablaAuxiliar->sgte_tabla;
//		}
//
//		//Ahora se le inserta el registro AL FINAL
//
//		ptr_nodo_registro RegistroAuxiliar;
//
//			TablaAuxiliar->sgte_registro = RegistroAuxiliar;
//			while(RegistroAuxiliar->sgte_registro != NULL)
//			{
//				RegistroAuxiliar = RegistroAuxiliar->sgte_registro;
//			}
//			RegistroAuxiliar->sgte_registro = nuevoRegistro;
//}


