import { defineConfig } from 'vite'
import path from 'path'
import react from '@vitejs/plugin-react'
import { viteSingleFile } from "vite-plugin-singlefile"
import dotenv from 'dotenv';

// load environment variables for the theme that was passed in (light or dark)
dotenv.config({ path: `.env.${(process.env.THEME || 'dark').trim()}` });

export default defineConfig({
  plugins: [
    react(),
    viteSingleFile()
  ],
  optimizeDeps: {
  },
  resolve: {
    alias: [
      // make sure css files resolve to the correct version based on the theme
      { find: 'App.css',        replacement: path.resolve( __dirname, `./src/${process.env.VITE_CSS_PATH}` )  },
      { find: 'Highlight.css',  replacement: `highlight.js/styles/${process.env.VITE_HIGHLIGHT_THEME}.css`    },
    ]
  }
})
