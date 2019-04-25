#include "astar.h"

//Repetir:
//       - Buscar el cuadro con el coste F más bajo en la lista abierta(cuadro actual)
//         - Para cada uno de los 8 cuadros adyacentes al cuadro actual
//         - Almacenar el coste G del cuadro sucesor
//         - Si el cuadro sucesor está en la lista abierta
//         - Si no mejora al de la lista, descartamos el sucesor y continuamos con el siguiente
//         - Si el cuadro sucesor está en la lista cerrada
//         - Si no mejora al de la lista, descartamos el sucesor y continuamos con el siguiente
//         - Eliminar el cuadro sucesor de la lista abierta o la cerrada
//         - Cambiar el padre del cuadrado al cuadro actual
//         - Calcular la heurística del cuadro sucesor
//         - Añadir el cuadro sucesor a la lista abierta
//         - Añadir el cuadro actual a la lista cerrada
//         - Dejar de calcular cuando :
//       - Se añada el cuadro objetivo a la lista abierta en cuyo caso el camino ha sido encontrado, o
//         - Se falle en encontrar el cuadro objetivo y la lista abierta esté vacía(en este caso no hay
//           camino)