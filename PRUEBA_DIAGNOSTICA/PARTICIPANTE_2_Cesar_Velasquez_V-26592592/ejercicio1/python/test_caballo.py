#!/usr/bin/env python3
"""Pruebas unitarias del verificador de movimientos de caballo."""

import unittest

import caballo


class PruebasCaballo(unittest.TestCase):
    def comprobar(self, inicial, movimientos, esperado):
        _, mensaje = caballo.validar(inicial, movimientos.split())
        self.assertEqual(mensaje, esperado)

    def test_correcto_4(self):
        self.comprobar("a1", "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO")

    def test_falla_movimiento_3(self):
        self.comprobar("a1", "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3")

    def test_correcto_5(self):
        self.comprobar("a2", "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO")

    def test_falla_movimiento_1(self):
        self.comprobar("a2", "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1")

    def test_correcto_2(self):
        self.comprobar("a8", "Kb6 Kd7", "movimiento CORRECTO")

    def test_falla_movimiento_2(self):
        self.comprobar("a8", "Kb6 Kb7", "INCORRECTO falla en el movimiento 2")

    def test_correcto_10(self):
        movimientos = "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"
        self.comprobar("a1", movimientos, "movimiento CORRECTO")

    def test_letra_incorrecta(self):
        movimientos = "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"
        self.comprobar(
            "a1",
            movimientos,
            "movimiento INCORRECTO falla letra Q no pertenece a un caballo",
        )

    def test_falla_movimiento_9(self):
        movimientos = "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"
        self.comprobar("a1", movimientos, "INCORRECTO falla en el movimiento 9")

    def test_casilla_invalida(self):
        self.comprobar("a1", "Kz9", "INCORRECTO falla en el movimiento 1")

    def test_k_minuscula(self):
        self.comprobar(
            "a1",
            "kc2",
            "movimiento INCORRECTO falla letra k no pertenece a un caballo",
        )


if __name__ == "__main__":
    unittest.main(verbosity=2)
