pNode parse_expr(pToken *curr);
pNode parse_term(pToken *curr);
pNode parse_factor(pToken *curr);

// NIVEL 3 (Más abajo en el árbol): Números, variables y 'neg'
pNode parse_factor(pToken *curr) {
    pToken tok = *curr;

    if (tok == NULL || tok->kind == eof_) return NULL;

    // Caso 1: Operador unario negativo (ej. -4)
    if (tok->kind == neg) {
        *curr = tok->next; // Avanzamos al siguiente token
        pNode operand = parse_factor(curr); // Recursión por si hay doble negación (--4)
        // Lo ponemos a la izquierda (o derecha, según tu convención para unarios)
        return new_node(neg, tok->content, operand, NULL);
    }

    // Caso 2: Número o Variable (Nodos hoja)
    if (tok->kind == num || tok->kind == var) {
        *curr = tok->next; // Consumimos el token
        return new_node(tok->kind, tok->content, NULL, NULL);
    }

    return NULL; // Manejo de error sintáctico
}

// NIVEL 2 (Medio): Multiplicación y División
pNode parse_term(pToken *curr) {
    // 1. Lo primero que encontramos va a la IZQUIERDA
    pNode left = parse_factor(curr);

    // 2. Mientras sigan apareciendo '*' o '/', seguimos creciendo el árbol hacia arriba
    while (*curr != NULL && ((*curr)->kind == mult || (*curr)->kind == divs)) {
        pToken op = *curr;
        *curr = op->next; // Consumimos el operador

        // 3. Lo siguiente va a la DERECHA
        pNode right = parse_factor(curr);

        // 4. Creamos el padre uniendo izquierda y derecha.
        // Este nuevo padre se convierte en el 'left' para la siguiente iteración.
        left = new_node(op->kind, op->content, left, right);
    }

    return left;
}

// NIVEL 1 (Más arriba): Suma y Resta
pNode parse_expr(pToken *curr) {
    // 1. Todo lo que tenga mayor precedencia (*, /) se resuelve primero y queda a la IZQUIERDA
    pNode left = parse_term(curr);

    // 2. Mientras haya '+' o '-'
    while (*curr != NULL && ((*curr)->kind == sum || (*curr)->kind == res)) {
        pToken op = *curr;
        *curr = op->next; // Consumimos '+' o '-'

        // 3. El operando derecho también puede ser una multiplicación/división
        pNode right = parse_term(curr);

        // 4. El nodo '+' o '-' envuelve lo anterior y se vuelve la nueva raíz local
        left = new_node(op->kind, op->content, left, right);
    }

    return left;
}