# -*- coding: utf-8 -*-
#
# Copyright (C) 2015-2020 Hewlett Packard Enterprise Development LP
# Copyright (C) 2022 Universidad de Costa Rica (UCR)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

"""
Argument management module.
"""

import logging
from argparse import ArgumentParser


log = logging.getLogger(__name__)


def validate_args(args):
    """
    Check that arguments are valid.

    :param args: An arguments namespace.
    :type args: :py:class:`argparse.Namespace`
    :return: The validated namespace.
    :rtype: :py:class:`argparse.Namespace`
    """

    log.debug('Raw arguments:\n{}'.format(args))

    return args


def parse_args(argv=None):
    """
    Argument parsing routine.

    :param list argv: A list of argument strings.
    :return: A parsed and verified arguments namespace.
    :rtype: :py:class:`argparse.Namespace`
    """

    parser = ArgumentParser(
        description=(
            'sensor_commands is an example python package for the '
            'IE0417 course @ EIE, UCR'
        )
    )

    parser.add_argument(
        '-c', '--config',
        default="config/sensors_cfg.json",
        help='Sensors configuration file',
    )
    parser.add_argument(
        '-t', '--sensor_type',
        default="temperature",
        help='Sensor type to read',
    )

    args = parser.parse_args(argv)
    args = validate_args(args)
    return args


__all__ = [
    'parse_args',
]
