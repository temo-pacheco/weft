# Literate Driven Development
### Un manifiesto

> *«En vez de imaginar que nuestra tarea principal es instruir a una
> computadora sobre qué hacer, concentrémonos más bien en explicar a seres
> humanos qué queremos que una computadora haga.»* — Donald Knuth
>
> *«El programador literario escribe documentación que contiene código. Es
> la diferencia entre ejecutar un truco de magia y exponerlo.»* — Ross Williams


## La tesis, en una frase

**TDD** dice: *el test es la especificación; el código es lo derivado.*

**Literate Driven Development (LDD)** dice:

> **La narrativa es la especificación, y tanto el código como los tests son
> artefactos derivados de ella.**

Esa inversión no es retórica. En programación literaria el archivo `.weft`
es, al mismo tiempo, tres cosas que en todos los demás flujos están
separadas y se desincronizan: el **spec**, la **ventana de contexto** y el
**entregable**. Prompt, código y documentación no son tres archivos: son un
solo objeto. LDD es la disciplina que convierte ese hecho en garantías de
ingeniería.


## Por qué ahora: la IA cambia la ecuación

Durante cuarenta años la programación literaria fue admirada y poco usada.
El motivo era económico: escribir la narrativa costaba tiempo humano, y ese
coste se pagaba por adelantado. La IA invierte la economía.

- La IA es cara en lo que **no** puede verificar y barata en lo que **sí**.
  La narrativa le da un sustrato verificable en cada nivel.
- La IA escribe prosa rápido. El cuello de botella histórico de LP —redactar
  la exposición— deja de serlo.
- La IA razona mejor sobre unidades pequeñas y con nombre que sobre archivos
  de dos mil líneas. El *chunk* es exactamente esa unidad.

LP y la IA no se toleran: se necesitan. LP le da a la IA lo que más le falta
—contexto autoritativo y verificación— y la IA le quita a LP su único
defecto —el coste de escribir.


## Los cuatro principios

**1 · La narrativa es el prompt.**
El humano escribe la intención en prosa y nombra los chunks. Ese acto de
nombrar *es* la tarea. La IA no adivina un diseño; rellena una
especificación que queda versionada en la fuente.

**2 · El chunk es la unidad atómica de todo.**
Un chunk es a la vez la unidad cognitiva (una pantalla, una idea), la unidad
de trabajo de la IA, la unidad de revisión humana y la unidad de commit. LDD
colapsa *tarea = chunk = revisión = transacción*. Ninguna IA edita dos mil
líneas; edita un chunk con su prosa.

**3 · El toolkit es el IDE de la IA — nunca el grep.**
La IA jamás navega ni edita el código generado. Navega la fuente con el mapa,
extrae fragmentos exactos, traza quién los usa y traduce errores de compilador
de vuelta a la línea `.weft`. El fallo clásico —«editó el archivo
generado»— se vuelve imposible por construcción.

**4 · La coherencia prosa↔código es una obligación, no una costumbre.**
El «terminado» incluye la prosa actualizada. Un chunk cambiado sin actualizar
su prosa no está terminado: se editó la capa derivada y se dejó la fuente
mintiendo. Es el mismo pecado que editar el código generado.


## El método

Tres fases. Cada una entrega algo real antes de que empiece la siguiente:

```
  FASE 1 · Enmarcar el dominio    (en prosa, antes de una línea de código)
     01 determinar el dominio      02 analizarlo
     03 determinar la arquitectura 04 planear las etapas
     → un documento de dominio, en cualquier formato

  FASE 2 · Armar el proyecto      (convertir el pensamiento en libro)
     05 escribir los primeros capítulos   06 generar el primer documento
     → el primer libro tejido

  FASE 3 · Escribir el libro      (capítulo a capítulo)
     07 escribir el capítulo       08 revisar y avanzar
     → el código literario completo
```

Dentro de un capítulo, cada unidad de trabajo se mueve así —el análogo de
*red–green–refactor*:

```
  NARRAR      humano: prosa + nombres de chunk       ← el "spec"
  ORIENTAR    IA: mapa dirigido (barato en tokens)   ← contexto
  RELLENAR    IA: implementa cuerpos de chunk        ← "green"
  VERIFICAR   tangle → build/test; errores→fuente    ← determinista
  RECONCILIAR IA: lint + coherencia prosa/código     ← "refactor"
```

Los dos pasos que ningún otro paradigma tiene son **Orientar** y
**Reconciliar**. Orientar sustituye «leer quince archivos» por una consulta
al grafo. Reconciliar comprueba que la prosa *sigue describiendo* lo que el
código hace: es el refactor, pero sobre la coherencia narrativa.

Y una sola frontera: un capítulo no se cierra con huecos abiertos. No hay
deuda técnica diferida a la etapa tres; cada etapa es una *edición* del libro
—teje, compila y se lee completa para lo que existe— y la siguiente se escribe
sobre terreno firme.


## Las cuatro promesas

### A · Certeza sobre la incertidumbre
Una alucinación es generación no verificada. LDD acota la incertidumbre con
un gate en cada nivel: el linter rechaza cualquier nombre de chunk inventado
(y sugiere el correcto); el diff prueba que lo generado está en sync; los
*section markers* traducen cada error de compilador a la línea exacta de la
fuente. Depurar deja de ser adivinar.

### B · Complejidad domada
La unidad de comprensión es el **concepto**, no el archivo. Un concepto cruza
todas las capas y todos los lenguajes en una sola narrativa: el *contrato*
entre lenguajes se explica en el único lugar donde viven los bugs
multi-lenguaje. La IA consulta un mapa del sistema entero; nunca carga el
territorio.

### C · Barato, rápido, incremental
El coste en tokens de un cambio escala con el tamaño del **cambio**, no del
**sistema** —porque los nombres permiten referenciar sin cargar. El código
estable y probado se trata como una caja negra con nombre. Se construye
siempre en la frontera, sobre lo que ya está probado.

### D · Obras de arte
El mismo source produce, a la vez, el programa y un **libro tipografiado**:
capítulos, índice, referencias cruzadas, glosario, bibliografía, tablas,
figuras, estilo. Lo que la imprenta resolvió en cinco siglos se hereda
gratis. Un concepto no está terminado si no se lee de corrido como un buen
ensayo.


## El estándar

El listón es *calidad publicable*. Un lector debería poder entender el
sistema leyéndolo de principio a fin, como se lee un buen libro. No código
con comentarios: un ensayo cuidadosamente construido donde código y prosa se
iluminan mutuamente.

Escribir así no es un lujo que se paga con lentitud. El tiempo que cuesta
explicar se recupera —con intereses— en el que no se gasta depurando. Y con
la IA como co-autora, ese coste tiende a cero mientras la garantía permanece.


---

*Este manifiesto acompaña al skill `literate-driven-development` y a la
herramienta `weft`. Se construyó, como todo lo demás, en programación
literaria.*
