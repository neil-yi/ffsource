'use strict';

module.exports = function ( grunt ) {

    var fs = require("fs"),
        Util = {

            jsBasePath: '_src/',
            cssBasePath: 'themes/default/_css/',

            fetchScripts: function () {

                var sources = fs.readFileSync( "_examples/editor_api.js" );

                sources = /\[([^\]]+)\]/.exec( sources );

                sources = sources[1].replace( /'|"|\n|\t|\s/g, '' );

                sources = sources.split( "," );

                sources.forEach( function ( filepath, index ) {

                    sources[ index ] = Util.jsBasePath + filepath;

                } );

                return sources;

            },

            fetchStyles: function () {

                var sources = fs.readFileSync( this.cssBasePath + "umeditor.css" ),
                    filepath = null,
                    pattern = /@import\s+([^;]+)*;/g,
                    src = [];

                while ( filepath = pattern.exec( sources ) ) {

                    src.push( this.cssBasePath + filepath[ 1 ].replace( /'|"/g, "" ) );

                }

                return src;

            }

        },
        server = grunt.option('server') || 'php',
        encode = grunt.option('encode') || 'utf8',
        disDir = "dist/",
        banner = '/*!\n * UEditor Mini版本\n * version: <%= pkg.version %>\n * build: <%= new Date() %>\n */\n\n';

    //init
    ( function () {

        server = typeof server === "string" ? server.toLowerCase() : 'php';
        encode = typeof encode === "string" ? encode.toLowerCase() : 'utf8';

        disDir = 'dist/' + encode + '-' + server + '/';

    } )();

    grunt.initConfig( {
        pkg: grunt.file.readJSON('package.json'),
        concat: {
            js: {
                options: {
                    banner: banner + '(function(){\n\n',
                    footer: '\n\n})()'
                },
                src: Util.fetchScripts(),
                dest: disDir + '<%= pkg.name %>.js'
            },
            css: {
                src: Util.fetchStyles(),
                dest: disDir + 'themes/default/css/umeditor.css'
            }
        },
        cssmin: {
            options: {
                banner: banner
            },
            files: {
                expand: true,
                cwd: disDir + 'themes/default/css/',
                src: ['*.css', '!*.min.css'],
                dest: disDir + 'themes/default/css/',
                ext: '.min.css'
            }
        },
        uglify: {
            options: {
                banner: banner
            },
            dest: {
                src: disDir + '<%= pkg.name %>.js',
                dest: disDir + '<%= pkg.name %>.min.js'
            }
        },
        copy: {
            base: {
                files: [
                    {

                        src: [ 'themes/default/images/**', 'dialogs/**', 'lang/**' ],
                        dest: disDir

                    }
                ]
            },
            php: {

                expand: true,
                src: 'php/**',
                dest: disDir

            },
            jsp: {

                expand: true,
                src: 'jsp/**',
                dest: disDir

            },
            net: {

                expand: true,
                src: 'net/**',
                dest: disDir

            }
        },
        transcoding: {

            options: {
                charset: encode
            },
            src: [disDir + '**/*.html', disDir + '**/*.js', disDir + '**/*.css', disDir + '**/*.jsp', disDir + '**/*.java', disDir + '**/*.php', disDir + '**/*.ashx', disDir + '**/*.cs']

        }

    } );

    grunt.loadNpmTasks('grunt-contrib-concat');
    grunt.loadNpmTasks('grunt-contrib-cssmin');
    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-contrib-copy');
    grunt.loadNpmTasks('grunt-transcoding');

    grunt.registerTask('default', 'UEditor Mini build', function () {

        var tasks = [ 'concat', 'cssmin', 'uglify', 'copy:base', 'copy:'+server, 'transcoding' ];

        //config修改
        updateConfigFile();

        grunt.task.run( tasks );

    } );


    function updateConfigFile () {

        var filename = 'umeditor.config.js',
            file = grunt.file.read( filename ),
            path = server + "/",
            suffix = server === "net" ? ".ashx" : "."+server;

        file = file.replace( /php\//ig, path ).replace( /\.php/ig, suffix );

        //写入到dist
        if ( grunt.file.write( disDir + filename, file ) ) {

            grunt.log.writeln( 'config file update success' );

        } else {
            grunt.log.warn('config file update error');
        }

    }

};